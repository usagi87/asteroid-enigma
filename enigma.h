#ifndef ENIGMA_H
#define ENIGMA_H

#include <QObject>


class Enigma : public QObject
{	  
    Q_OBJECT

private:
	
	
public:
	enum SecurityTemplate{ TemplateMaximum=0,TemplateLong,TemplateMedium,
		TemplateBasic,TemplateShort,TemplatePIN,TemplateName,TemplatePhrase};
	
	Q_INVOKABLE std::string MasterPassword(std::string fullName,std::string secret);
	
	Q_INVOKABLE std::string SitePassword(std::string userName,std::string siteName, std::string siteCounter, std::string masterPassword);
	
	Q_INVOKABLE QString generateSitePassword(QString fullname,QString seceret,QString username,QString sitename,QString siteCounter);
	Q_INVOKABLE std::string hex2int(std::string hex);
	Q_INVOKABLE std::string int2hex(std::string text);
	
	Q_INVOKABLE std::string type_templates(SecurityTemplate type, uint8_t templateIndex);
	Q_INVOKABLE std::string class_characters(const char characterClass);
	Q_INVOKABLE std::string type_strings(uint8_t index, std::string* str);
	Q_INVOKABLE uint8_t* hmac_sha256(uint8_t *key, int keylength,uint8_t *salt,int saltlength);
	Q_INVOKABLE uint8_t* FL(uint8_t *P,int Plen, uint8_t *S, int Slen,int c,uint8_t id );
	Q_INVOKABLE uint8_t* PBKDF2(uint8_t *password, int plen,uint8_t *salt, int slen, int c, int dkLen);
			
		
};

#endif
