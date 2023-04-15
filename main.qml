/*
 * Copyright (C) 2016 Florent Revest <revestflo@gmail.com>
 *   2015 Tim Süberkrüb <tim.sueberkrueb@web.de>
 * Part of this code is based on "Stopwatch" (https://github.com/baleboy/stopwatch)
 * Copyright (C) 2011 Francesco Balestrieri
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.9
import QtQuick.Layouts 1.3
import org.asteroid.controls 1.0
import org.asteroid.utils 1.0
import Nemo.Configuration 1.0
import QtQuick.VirtualKeyboard 2.0
import QtQuick.VirtualKeyboard.Settings 2.15
import Enigma 1.0

Application {
	id: app
	
	centerColor: "#b01c7e"
	outerColor: "#420a2f"
 	
 	property var fullname:""
 	property var secret:""
 	property var username:""
 	property var sitename:""
 	property var counter:"1"
 	 	
 	Enigma{
 		id:enigma
 	}
 	
 	LayerStack {
   	id: layerStack
		firstPage: loginPage

	}

Component{
	id:loginPage
	Item{
	id:rootM
	TextField {
		id: secretField
		anchors.verticalCenter : parent.verticalCenter
		anchors.horizontalCenter : parent.horizontalCenter
		width: Dims.w(80)
		previewText: qsTrId("Enigma secret")
	}
	TextField {
		id: fullnameField
		anchors.horizontalCenter : parent.horizontalCenter
		anchors.bottom : secretField.top
		anchors.bottomMargin: 10
		width: Dims.w(80)
		previewText: qsTrId("Full name")
	}
			
	HandWritingKeyboard {
		anchors.fill: parent
	}
	
	IconButton {
		anchors.horizontalCenter: parent.horizontalCenter
      anchors.bottom: parent.bottom
     	anchors.bottomMargin: Dims.h(15)
		iconName: "ios-checkmark-circle-outline"
		onClicked: { 
			fullname = fullnameField.text
			secret = secretField
			layerStack.push(siteLoginPage)
 		}
	}
	}
}

Component{
	id:siteLoginPage
	Item{
	id:rootM
	
	TextField {
		id: siteNameField
		anchors.verticalCenter : parent.verticalCenter
		anchors.horizontalCenter : parent.horizontalCenter
		width: Dims.w(80)
		previewText: qsTrId("Site name")
	}
	TextField {
		id: userNameField
		anchors.horizontalCenter : parent.horizontalCenter
		anchors.bottom : siteNameField.top
		anchors.bottomMargin: 10
		width: Dims.w(80)
		previewText: qsTrId("User name")
	}
			
	HandWritingKeyboard {
		anchors.fill: parent
	}
	
	IconButton {
		anchors.horizontalCenter: parent.horizontalCenter
       	anchors.bottom: parent.bottom
       	anchors.bottomMargin: Dims.h(15)
		iconName: "ios-checkmark-circle-outline"
		onClicked: { 
			sitename = siteNameField.text
			username = userNameField.text
			layerStack.push(sitePasswdPage)
 		}
	}
	
	}
}

Component{
	id:sitePasswdPage
	Item{
		id:rootM
		TextField {
			id: sitePasswdField
			anchors.horizontalCenter : parent.horizontalCenter
			anchors.verticalCenter : parent.verticalCenter
			width: Dims.w(80)
			text: enigma.generateSitePassword(fullname,secret,username,sitename,counter) 		
		}	
	}
}


Component {
	id:passwdRulesPage
	Item {
	
	PageHeader {
    	id: title
        text: "Password rules:"
    }

	Flickable {
		anchors.topMargin: Dims.h(10)
        anchors.fill: parent
        contentHeight: Dims.h(30) + 4*Dims.h(25) + (DeviceInfo.needsBurnInProtection ? Dims.h(34) : 0)
        boundsBehavior: Flickable.DragOverBounds
        flickableDirection: Flickable.VerticalFlick
				
		GridLayout {
            id: rulesSettingsField
            columns: 2
            anchors.fill: parent
            anchors.margins: Dims.l(5)
			
			Label {
                text: "Upper case letter[A-Z]"
                font.pixelSize: Dims.l(6)
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
                Layout.maximumWidth: Dims.w(70)
            }

            Switch {
            	id:upperSelect
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                width: Dims.l(15)
                checked: upperSelect.checked
                onCheckedChanged: upperSelect.checked = checked
            }

            Label {
                text: "Lower case letter[a-z]"
                font.pixelSize: Dims.l(6)
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
                Layout.maximumWidth: Dims.w(70)
            }

            Switch {
            	id:lowerSelect
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                width: Dims.l(15)
                checked: lowerSelect.checked
                onCheckedChanged: lowerSelect.checked = checked
            }	
            
            Label {
                text: "Number[0-9]"
                font.pixelSize: Dims.l(6)
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
                Layout.maximumWidth: Dims.w(70) 
            }

            Switch {
            	id:numberSelect
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                width: Dims.l(15)
                checked: numberSelect.checked
                onCheckedChanged: numberSelect.checked = checked
            }

            Label {
            	text: "Symbol[@&%?,=[]_:-+*$#!'^~;()/.]"
                font.pixelSize: Dims.l(6)
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
                Layout.maximumWidth: Dims.w(70)
            }

            Switch {
            	id:symbolSelect
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                width: Dims.l(15)
                checked: symbolSelect.checked
                onCheckedChanged: symbolSelect.checked = checked
            }
			Label {
            	text: "Symbol[@&%?,=[]_:-+*$#!'^~;()/.]"
                font.pixelSize: Dims.l(6)
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
                Layout.maximumWidth: Dims.w(70)
            }

            Switch {
            	id:symbolSelect1
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                width: Dims.l(15)
                checked: symbolSelect.checked
                onCheckedChanged: symbolSelect.checked = checked
            }
			Label {
            	text: "Symbol[@&%?,=[]_:-+*$#!'^~;()/.]"
                font.pixelSize: Dims.l(6)
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
                Layout.maximumWidth: Dims.w(70)
            }

            Switch {
            	id:symbolSelect2
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                width: Dims.l(15)
                checked: symbolSelect.checked
                onCheckedChanged: symbolSelect.checked = checked
            }
        	Item{
        		id: rulesSettingsButton
            	height: Dims.h(30)
                Layout.fillWidth: true
                Layout.columnSpan: 1
				IconButton {
					anchors.horizontalCenter: parent.horizontalCenter
       				anchors.bottom: parent.bottom
					iconName: "ios-checkmark-circle-outline"
					onClicked: { 
										
 					}
				}        		
        	}
        }
    }
	}
}

}


