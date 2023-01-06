//==========================================================
//
// rssRead
// Read rss (xml) and return an array of strings.
// date/author    : 2022/07/13 @chrmlinux03
// update/author  : 2023/01/05 @kylefmohr
// LICENSE        : MIT
// Version        : 0.0.1 Test Version
//                : 0.0.2 example -> examples
//                : 0.1.0 memory full fix
//                : 0.1.1 rename variables and add comments for clarity
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
    // Use this in setup().
    // OPTIONAL PARAMETER: bufSize, int, default: 1024 * 20 (20KB)
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
    // Can be used when you want to free memory.
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
    // tagIndex
    // Return value: the index of the tag that will be returned by the next finds() call.
    //
    // OPTIONAL PARAMETER: tagIndex, int, default: 0
    // Set the index of the tag that will be returned by the next finds() call.
    //
    //=================================================
    uint16_t tagIndex(void) {
      return tagIndex(_tagIndex);
    }
    uint16_t tagIndex(uint16_t tagIndex) {
      _tagIndex = tagIndex;
      return _tagIndex;
    }

    //=================================================
    //
    // open
    // Pass the URL of the RSS feed to be read.
    // Return value: 0: OK, -1: Connection failed
    //
    //=================================================
    int16_t open(const char *url) {
      int16_t returnCode = 0;

      //-------------------------------------------------
      // url2host
      //-------------------------------------------------
      String host = url2host(String(url));

      https.setInsecure();
      if (!https.connect(host.c_str(), _port)) {
        //-------------------------------------------------
        // connect failed
        //-------------------------------------------------
        returnCode = -1;
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
      return returnCode;
    }

    //=================================================
    //
    // finds
    // Required parameter: tag, String
    // Pass the tag of the value that you'd like to get
    // Returns a String of the first value found, then the next value on subsequent calls
    //
    //=================================================
    String finds(String tag) {
      static int32_t position = 0;
      int32_t start, end;
      static int32_t sst = -1;
      String value = "";

      start = _xml.indexOf(tag, position) + tag.length();
      end = _xml.indexOf(tag, start);
      if (sst < start) {
        sst = start;
        value = _xml.substring(start + 1, end - 2);
        position = end + tag.length();
        _tagIndex ++;
      }
      return value;
    }

    //=================================================
    //
    // dumpXml
    // Dump the XML data for debugging
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
      String value = "";
      int start, end;
      start = src.indexOf(tgt) + tgt.length();
      end = src.indexOf("/", start);
      value = src.substring(start, end);
      return value;
    }

    //=================================================
    //
    // default values
    //
    //=================================================
    String _xml = "";
    uint32_t _bufSize = __RSSREAD_DEFXMLSIZE__;
    uint16_t _port = __RSSREAD_DEFPORT__;
    uint32_t _bufPos = 0;
    uint16_t _tagIndex = 0;
};

#endif
