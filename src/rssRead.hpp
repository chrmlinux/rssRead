//==========================================================
//
// rssRead
// Read rss (xml) and return an array of strings.
// date/author    : 2022/07/13 @chrmlinux03
// update/author  : 2022/07/14 @chrmlinux03
// LICENSE        : MIT
// Version        : 0.0.1
//==========================================================

#ifndef __RSSREAD_HPP__
#define __RSSREAD_HPP__

#include <Arduino.h>
#include <string.h>
#include <WiFiClientSecure.h>
static WiFiClientSecure https;

#define __RSSREAD_DEFXMLSIZE__ (1024 * 10)
#define __RSSREAD_HOSTLENGTH__ (128)
#define __RSSREAD_SENDLENGTH__ (256)

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
    // axs
    //
    //=================================================
    int16_t axs(const char *url) {
      int16_t rtn = 0;

      //-------------------------------------------------
      // url2host
      //-------------------------------------------------
      char host[__RSSREAD_HOSTLENGTH__] = {0};
      strcpy(host, url2host(String(url)).c_str());

      https.setInsecure();
      if (!https.connect(host, 443)) {
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
        char snd[__RSSREAD_SENDLENGTH__] = {0};

        //-------------------------------------------------
        // server  connection
        //-------------------------------------------------
        sprintf(snd, "GET %s HTTP/1.0", url); https.println(snd);
        sprintf(snd, "Host: %s", host); https.println(snd);
        https.println("Connection: close");
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
        }
        https.stop();
        _xml = String(buf);
        free(buf); // free
      }
      return rtn;
    }

    //=================================================
    //
    // finds
    //
    //=================================================
    String finds(String tag) {
      static int32_t pos = 0;
      int32_t st, en;
      static int32_t sst = -1;
      String dst = "";

      st = _xml.indexOf(tag, pos) + tag.length();
      en = _xml.indexOf(tag, st);
      if (sst < st) {
        sst = st;
        dst = _xml.substring(st + 1, en - 2);
        pos = en + tag.length();
        _tagCnt ++;
      }
      return dst;
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
    uint32_t _bufPos = 0;
    uint16_t _tagCnt = 0;
};

#endif
