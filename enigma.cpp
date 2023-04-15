#include "enigma.h"
#include <iostream>
#include <string>
#include <QDebug>
#include <scrypt/crypto_scrypt.h>
#include "scrypt/scrypt-kdf.h"
#include "arduinolibs/libraries/Crypto/SHA256.h"
#include "arduinolibs/libraries/Crypto/Hash.h"
#include "arduinolibs/libraries/Crypto/Crypto.h"
#include "arduinolibs/libraries/Crypto/BLAKE2b.h"
#include "arduinolibs/libraries/Crypto/HKDF.h"
#include <sstream>
#include <cstdlib>
#include <iomanip>

using namespace std;

	const int N = 65536;
	const int r = 8;
	const int p = 1;
	
	const int cc = 10000;

string Enigma::MasterPassword(string fullName, string secret){
	
	const char * username = fullName.c_str();
	const char * password = secret.c_str();	
	
	int OUTPUT_BUFLEN = 64;
	uint8_t output[OUTPUT_BUFLEN];
	int exitcode;
	
	exitcode = scrypt_kdf((const uint8_t *)password, fullName.size(),
	    (const uint8_t*)username, secret.size(), N, r, p, output, OUTPUT_BUFLEN);
	
	string str = "";
	for (int i=0;i<64;++i) str += output[i];
		
	return str;
}

string Enigma::SitePassword(string userName,string siteName, string siteCounter, string masterPassword){
	uint8_t klen = masterPassword.size();
	uint8_t key[klen];
	for(int i=0;i<klen;++i) key[i] = (uint8_t )masterPassword[i];
	
	uint8_t ulen = userName.size();
	uint8_t user[ulen];
	for(int i=0;i<ulen;++i) user[i] = (uint8_t)userName[i];
	
	uint8_t slen = siteName.size();
	uint8_t site[slen];
	for(int i=0;i<slen;++i) site[i] = (uint8_t)siteName[i];
	
	uint8_t clen = siteCounter.size();
	uint8_t count[clen];
	for(int i=0;i<clen;++i) count[i] = (uint8_t)siteCounter[i];
	
	int OUTPUT_BUFLEN = 32;
	uint8_t output[OUTPUT_BUFLEN];
	
	SHA256 sha256;
	sha256.resetHMAC(key, 32);
	sha256.update(user, ulen);
	sha256.update(site, slen);
	sha256.finalizeHMAC(key, 32, output, OUTPUT_BUFLEN);
	
	uint8_t *tmp = PBKDF2(output,OUTPUT_BUFLEN,count,clen, cc, 32);

	string str = "";
	for(int i=0;i<OUTPUT_BUFLEN;++i) str += tmp[i];
	
	return str;
}

QString Enigma::generateSitePassword(QString fullname,QString secret,QString username,QString sitename,QString siteCounter) {
	
	string masterPasswd = MasterPassword(fullname.toStdString(),secret.toStdString());
	string sitePasswd = SitePassword(username.toStdString(),sitename.toStdString(),siteCounter.toStdString(),masterPasswd); 
	
	const uint8_t templateIndex = (uint8_t)sitePasswd[0];
	string templates = type_templates(TemplateLong,templateIndex);
	int template_size = templates.size();
	string password = "";
	
	for(int i=0;i<template_size;++i){
		string passChar = class_characters(templates[i]);
		password += passChar[sitePasswd[i+1] % passChar.size()]; 
	}
	
	return QString::fromStdString(password);
}

string Enigma::class_characters(const char characterClass) {

    switch (characterClass) {
        case 'V':
            return "AEIOU";
        case 'C':
            return "BCDFGHJKLMNPQRSTVWXYZ";
        case 'v':
            return "aeiou";
        case 'c':
            return "bcdfghjklmnpqrstvwxyz";
        case 'A':
            return "AEIOUBCDFGHJKLMNPQRSTVWXYZ";
        case 'a':
            return "AEIOUaeiouBCDFGHJKLMNPQRSTVWXYZbcdfghjklmnpqrstvwxyz";
        case 'n':
            return "0123456789";
        case 'o':
            return "@&%?,=[]_:-+*$#!'^~;()/.";
        case 'x':
            return "AEIOUaeiouBCDFGHJKLMNPQRSTVWXYZbcdfghjklmnpqrstvwxyz0123456789!@#$%^&*()";
        case ' ':
            return " ";
        default: 
            return "Unknown character class.";
    }
}

string Enigma::type_templates(SecurityTemplate type, uint8_t templateIndex) {
	
	switch(type) {
		case TemplateMaximum:
			return type_strings( templateIndex,new string[]{"anoxxxxxxxxxxxxxxxxx", "axxxxxxxxxxxxxxxxxno",";"});
		break;
		case TemplateLong:
			return type_strings( templateIndex,new string[]{"CvcvnoCvcvCvcv", "CvcvCvcvnoCvcv", "CvcvCvcvCvcvno","CvccnoCvcvCvcv", "CvccCvcvnoCvcv", "CvccCvcvCvcvno", "CvcvnoCvccCvcv", "CvcvCvccnoCvcv", "CvcvCvccCvcvno","CvcvnoCvcvCvcc", "CvcvCvcvnoCvcc", "CvcvCvcvCvccno","CvccnoCvccCvcv","CvccCvccnoCvcv", "CvccCvccCvcvno","CvcvnoCvccCvcc", "CvcvCvccnoCvcc", "CvcvCvccCvccno","CvccnoCvcvCvcc", "CvccCvcvnoCvcc", "CvccCvcvCvccno",";"});
		break;
		case TemplateMedium:
			return type_strings( templateIndex,new string[]{"CvcnoCvc", "CvcCvcno", ";"});
		break;
		case TemplateBasic:
			return type_strings( templateIndex,new string[]{"aaanaaan", "aannaaan", "aaannaaa",";"});
		break;
		case TemplateShort:
			 return type_strings( templateIndex,new string[]{"Cvcn",";"});
		break;
		case TemplatePIN:
			return type_strings( templateIndex,new string[]{"nnnn",";"});
		break;
		case TemplateName:
			return type_strings( templateIndex,new string[]{"cvccvcvcv",";"});
		break;
		case TemplatePhrase:
			return type_strings(templateIndex,new string[]{"cvcc cvc cvccvcv cvc", "cvc cvccvcvcv cvcv", "cv cvccv cvc cvcvccv",";"});
		break;
		default:
			return ";";
	}
	return NULL;
}


string Enigma::type_strings(uint8_t index, string* str){
	uint8_t slen = 0;
	for(int i=0; str[i] != ";";++i) slen++;
	
	string tmp = str[index%slen];
	delete[] str; 
	return tmp;
}


uint8_t* Enigma::PBKDF2(uint8_t *password, int plen,uint8_t *salt, int slen, int c, int dkLen){
	int hLen = 32;
	int l = (dkLen + hLen - 1) / hLen;
	uint8_t *mac = new uint8_t[32];

	for (int i = 1; i <= l; i++) {
		mac = FL(password, plen,salt, slen, c, i);
	} 
	
	return mac;
}

uint8_t* Enigma::FL(uint8_t *P,int Plen, uint8_t *S, int Slen,int c,uint8_t id ){
	uint8_t *msg = new uint8_t[32];
	uint8_t mac[32];
	uint8_t state[32];
  
  	for(int i=0;i<32;++i) mac[i] = 0;
	
	uint8_t idBigEndian[4] = {0,0,0,id};
	//idBigEndian[0] = (uint8_t)((int)id >> 24);
  	//idBigEndian[1] = (uint8_t)((int)id >> 16);
  	//idBigEndian[2] = (uint8_t)((int)id >> 8);
  	//idBigEndian[3] = (uint8_t)id;

	
	SHA256 sha256;
	sha256.resetHMAC(P, Plen);
	sha256.update(S, Slen);
	sha256.update(idBigEndian, 4);
	sha256.finalizeHMAC(P, Plen, state, sizeof(state));
	
	for(int j=0;j<32;++j){
      	mac[j] ^= state[j];
   }
   
	for(int i=1;i != c;++i){
		msg = hmac_sha256(P, Plen, state,sizeof(state));
		for(int j=0;j<32;++j){
      	mac[j] ^= msg[j];
      	state[j] = msg[j];
   	}
  } 
  
  for(int i=0;i<32;++i ) msg[i] = mac[i];  
  return msg;
}

uint8_t* Enigma::hmac_sha256(uint8_t *key, int keylength,uint8_t *salt,int saltlength){
	
	int OUTPUT_BUFLEN = 32;
	uint8_t output[OUTPUT_BUFLEN];
	SHA256 sha256;
	sha256.resetHMAC(key, keylength);
	sha256.update(salt, saltlength);
	sha256.finalizeHMAC(key, keylength, output, OUTPUT_BUFLEN);

	uint8_t *mac = new uint8_t[32];
	for(int i=0;i<32;++i) mac[i] = output[i];	
	return mac;
}

string Enigma::int2hex(string text){
	stringstream ss;
	ss.clear();
	for (auto str : text) 
	{
        ss << uppercase <<setfill('0') << setw(2) << hex << (int)str;
    }

	return ss.str();
}

string Enigma::hex2int(string hex){
	string tmp = "";
	for(unsigned int i=0;i<hex.size();i+=2){
		tmp += (char)stoi(hex.substr(i,2),0,16);
	}
	return tmp;
}
