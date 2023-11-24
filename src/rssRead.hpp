//==========================================================
//
// rssRead
// Read rss (xml) and return an array of strings.
// date/author    : 2022/07/13 @chrmlinux03
// update/author  : 2022/07/14 @chrmlinux03
// LICENSE        : MIT
// Version        : 0.0.1 Test Version
//                : 0.0.2 example -> examples
//                : 0.1.0 memory full fix
//==========================================================

#ifndef __RSSREAD_HPP__
#define __RSSREAD_HPP__

#include <Arduino.h>
#include <string.h>
#include <WiFiClientSecure.h>
static WiFiClientSecure https;

#define __RSSREAD_DEFXMLSIZE__ (1024 * 20)
#define __RSSREAD_DEFPORT__ (443)

//==========================================================
//
// rssRead
//
//==========================================================
class rssRead {
  public:
    //=================================================
    //
    // public
    //
    //=================================================

    //=================================================
    //
    // construct
    //
    //=================================================
    rssRead(void) {}
    ~rssRead(void) {}

    //=================================================
    //
    // begin
    //
    //=================================================
    void begin(void) {
      begin(_bufSize);
    }
    void begin(uint32_t bufSize) {
      _bufSize = bufSize;
    }

    //=================================================
    //
    // end
    //
    //=================================================
    void end(void) {
      _bufPos = 0;
      _xml = "";
    }

    //=================================================
    //
    // bufPos
    //
    //=================================================
    uint32_t bufPos(void) {
      return bufPos(_bufPos);
    }
    uint32_t bufPos(uint32_t bufPos) {
      _bufPos = bufPos;
      return _bufPos;
    }

    //=================================================
    //
    // tagCnt
    //
    //=================================================
    uint16_t tagCnt(void) {
      return tagCnt(_tagCnt);
    }
    uint16_t tagCnt(uint16_t tagCnt) {
      _tagCnt = tagCnt;
      return _tagCnt;
    }

    //=================================================
    //
    // findPos
    //
    //=================================================
    int32_t findPos(void) {
      return _pos;
    }
    int32_t findPos(int32_t pos) {
      _pos = pos;
      return _pos;
    }
     //=================================================
    //
    // start
    //
    //=================================================
    int32_t start(void) {
      return _sst;
    }
    int32_t start(int32_t sst) {
      _sst = sst;
      return _sst;
    }
    //=================================================
    //
    // axs
    //
    //=================================================
    int16_t axs(const char *url) {
      int16_t rtn = 0;
      _pos = 0;
      _sst = -1;
      //-------------------------------------------------
      // url2host
      //-------------------------------------------------
      String host = url2host(String(url));

      https.setInsecure();
      if (!https.connect(host.c_str(), _port)) {
        //-------------------------------------------------
        // connect failed
        //-------------------------------------------------
        rtn = -1;
      } else {
        //-------------------------------------------------
        // connect OK
        //-------------------------------------------------
        char *buf;
        buf = new char[_bufSize]; // alloc
        memset(buf, 0x0, _bufSize);

        //-------------------------------------------------
        // server  connection
        //-------------------------------------------------
        https.println("GET " + String(url) + " HTTP/1.0");
        https.println("Host: " + host);
//        https.println("Connection: close");
        https.println("Connection: Keep-Alive");
        https.println();

        //-------------------------------------------------
        // recv header
        //-------------------------------------------------
        while (https.connected()) {
          String line = https.readStringUntil('\n');
          if (line == "\r") break;
        }

        //-------------------------------------------------
        // recv data
        //-------------------------------------------------
        _bufPos = 0;
        while (https.available()) {
          buf[_bufPos] = https.read();
          _bufPos ++;
          if (_bufPos > (_bufSize - 1)) break;
        }
        https.stop();
        _xml = String(buf);
        free(buf); // free
        _xml.replace("&lt;", "<");
        _xml.replace("&gt;", ">");
        _xml.replace("&#39;", "'");
//        _xml.replace("&quot;", """);
        _xml.replace("&amp;", "&");
        _xml.replace("&nbsp;", " ");
      }
      return rtn;
    }

    //=================================================
    //
    // finds
    //
    //=================================================
    String finds(String tag) {
      int32_t st, en;
      
      String dst = "";

      st = _xml.indexOf(tag, _pos) + tag.length();
      en = _xml.indexOf(tag, st);
      if (_sst < st) {
        _sst = st;
        dst = _xml.substring(st + 1, en - 2);
        _pos = en + tag.length();
        _tagCnt ++;
      }
      return dst;
    }

    //=================================================
    //
    // dumpXml
    //
    //=================================================
    void dumpXml(void) {
      Serial.println("=== dumpXml Start ===>>");
      Serial.write(_xml.c_str(), _bufPos);
      Serial.println("\n<<=== dumpXml End ===");
    }

  private:
    //=================================================
    //
    // private
    //
    //=================================================

    //=================================================
    //
    // url2host
    //
    //=================================================
    String url2host(String src) {
      String tgt = "https://";
      String dst = "";
      int st, en;
      st = src.indexOf(tgt) + tgt.length();
      en = src.indexOf("/", st);
      dst = src.substring(st, en);
      return dst;
    }

    //=================================================
    //
    // work
    //
    //=================================================
    String _xml = "";
    uint32_t _bufSize = __RSSREAD_DEFXMLSIZE__;
    uint16_t _port = __RSSREAD_DEFPORT__;
    uint32_t _bufPos = 0;
    uint16_t _tagCnt = 0;
    int32_t _pos = 0;
    int32_t _sst = -1;
};

#endif
