//==========================================================
//
// rssRead
// Read rss (xml) and return an array of strings.
//
// date/author    : 2022/07/13 @chrmlinux03
// update/author  : 2022/07/14 @chrmlinux03
// LICENSE        : MIT
//
//==========================================================

#include "rssRead.hpp"
static rssRead rss;
//const char *url  = "https://news.yahoo.co.jp/rss/topics/top-picks.xml";
const char *url = "https://www.nhk.or.jp/rss/news/cat0.xml";
//const char *url = "https://www.asahi.com/rss/asahi/newsheadlines.rdf";
//const char *url = "https://mainichi.jp/rss/etc/mainichi-flash.rss";
//const char *url = "https://news.google.com/rss/search?q=Japan&hl=ja&gl=JP&ceid=JP:ja";

//-------------------------------------------------
// WiFiConnect
//-------------------------------------------------
const char *ssid = "********";
const char *pswd = "********";
void WiFiConnect(void) {
  WiFi.disconnect(true, true);
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
}

//-------------------------------------------------
// setup
//-------------------------------------------------
void setup(void) {

  Serial.begin(115200); while (!Serial);
  WiFiConnect();
  Serial.println("Start rssRead ==>");
  rss.begin();
  rss.axs(url);
//  rss.dumpXml();
  
  while(1) {
    String dst = rss.finds(String("title"));
    if (!dst.length()) break;
    Serial.printf("[%s]\n", dst.c_str());
  }
  Serial.printf("<== End rssRead(%d)\n", rss.tagCnt());
}

//-------------------------------------------------
// loop
//-------------------------------------------------
void loop(void) {
}
