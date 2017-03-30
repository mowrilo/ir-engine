#include <CkSpider.h>
#include <iostream>

int main()
    {
    CkSpider spider;

    //  The spider object crawls a single web site at a time.  As you'll see
    //  in later examples, you can collect outbound links and use them to
    //  crawl the web.  For now, we'll simply spider 10 pages of chilkatsoft.com
    spider.Initialize("www.xvideos.com");

    //  Add the 1st URL:
    spider.AddUnspidered("http://www.xvideos.com/");

    //  Begin crawling the site by calling CrawlNext repeatedly.
    int i;
    for (i = 0; i <= 20; i++) {
        bool success;
        success = spider.CrawlNext();
        if (success == true) {
            //  Show the URL of the page just spidered.
            std::cout << spider.lastUrl() << "\r\n";
            spider.AddUnspidered(spider.lastUrl());
            //  The HTML is available in the LastHtml property
        }
        else {
            //  Did we get an error or are there no more URLs to crawl?
            if (spider.get_NumUnspidered() == 0) {
                std::cout << "No more URLs to spider" << "\r\n";
            }
            else {
                std::cout << spider.lastErrorText() << "\r\n";
            }

        }

        //  Sleep 1 second before spidering the next URL.
        spider.SleepMs(2000);
    }

    return 0;
    }
