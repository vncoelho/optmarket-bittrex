#ifndef BITTREX_API_HPP
#define BITTREX_API_HPP

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <cstring>
#include <sstream>
//#include <crypt.h>

#include <curl/curl.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>

using namespace std;

extern string API_PUBLIC_KEY;
extern string API_PRIVATE_KEY;

//Options for signing message with your private key and/or exporting json to file or string
struct InstuctionOptions
{
	bool printAnswer, exportBuffer, signMessage;
	InstuctionOptions(bool _printAnswer = true, bool _signMessage = false, bool _exportBuffer = false) :
			printAnswer(_printAnswer), signMessage(_signMessage), exportBuffer(_exportBuffer)
	{

	}
};

class BittrexAPI
{
private:
	stringstream ssURL;
	double feePerOrder = 0.0025; //market fees for order = 0,25%

public:

	BittrexAPI()
	{

	}

	virtual ~BittrexAPI()
	{
	}

	double getFeesPerOrder()
	{
		return feePerOrder;
	}
	string getHMAC2(string keyParam, string message)
	{
		//	    char data[] = "Hi! Donate us some coins";
		//		char key[] = "BTC:17GrXw3qdz1G6DRxvtmhnhLzFXjRK5z6NY";
		//		char key[] = "ETH:0xd35fb76723636e11Fa580665A54E19F781C6Cd8a";
		//		char key[] = "ANS:AZZZsZJVqZwfHUx55p6UdY1Vx7qCjhnjMy";

		char key[10000]; //todo huge size for avoiding smashing -- not efficent but works
		char data[10000];
		strcpy(key, keyParam.c_str());
		strncpy(data, message.c_str(), sizeof(data));

		unsigned char* digest;

		//============================================
		//Example extract from http://www.askyb.com/cpp/openssl-hmac-hasing-example-in-cpp/
		//askyb on February, 26th 2013 in C++ OpenSSL
		//=============================================
		// Using sha1 hash engine here.
		// You may use other hash engines. e.g EVP_md5(), EVP_sha224, EVP_sha512, etc
		digest = HMAC(EVP_sha512(), key, strlen(key), (unsigned char*) data, strlen(data), NULL, NULL);

		// Be careful of the length of string with the choosen hash engine.
		//SHA1 produces a 20-byte hash value which rendered as 40 characters.
		// Change the length accordingly with your choosen hash engine
		char mdString[SHA512_DIGEST_LENGTH];
		for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
			sprintf(&mdString[i * 2], "%02x", (unsigned int) digest[i]);

		//printf("HMAC digest: %s\n", mdString);
		//=============================================

		string output = mdString;
		return output;
	}

	static size_t writerToString(void *contents, size_t size, size_t nmemb, void *userp)
	{
		((string*) userp)->append((char*) contents, size * nmemb);
		size_t realsize = size * nmemb;
		return realsize;
	}

	string callCurlPlataform(InstuctionOptions instOpt)
	{
		//For more information: check: https://curl.haxx.se/libcurl/c/getinmemory.html
		if (ssURL.str() == "")
		{
			cout << "empty instruction." << endl;
			getchar();
			return "";
		}
		else
		{
			cout << "\n=============================================" << endl;
			cout << "Executing instruction\t Signed:" << instOpt.signMessage << "\n" << ssURL.str() << endl;
//			cout << "press any key to continue..." << endl;
			cout << "=============================================" << endl;
//			getchar();
		}

		CURL *curl_handle;

		string readBuffer;

		curl_global_init(CURL_GLOBAL_ALL);

		// curl session begins
		curl_handle = curl_easy_init();

		// set destination URL
		curl_easy_setopt(curl_handle, CURLOPT_URL, (const char* ) ssURL.str().c_str());

		/* signing instruction */
		if (instOpt.signMessage == true)
		{
			string signature = getHMAC2(API_PRIVATE_KEY, ssURL.str());
			cout << "\n----------------------------------------------" << endl;
			cout << "HMAC signature (header purpose): \n" << signature << endl;
			cout << "----------------------------------------------" << endl;
			stringstream ssSignature;
			ssSignature << "apisign:" << signature;

			struct curl_slist *headerlist = NULL;
			headerlist = curl_slist_append(headerlist, (const char*) ssSignature.str().c_str());

			//setting header with signature
			curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headerlist);
		}

		curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

		// send all data to this function
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writerToString);
		// and filling a string buffer
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &readBuffer);

		//let's get it
		CURLcode ans = curl_easy_perform(curl_handle);

		/* check for errors */
		if (ans != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() found problems (failed): %s\n", curl_easy_strerror(ans));
			exit(1);
		}
		else
		{
			cout << "\n=============================================" << endl;
			cout << "=============================================" << endl;
			cout << "Server answer:" << endl;
			if (instOpt.printAnswer)
				cout << readBuffer << endl;
			else
				cout << "message optionally omitted" << endl;

			cout << "=============================================\n" << endl;
			//		getchar();

		}

		if (instOpt.exportBuffer)
		{
			cout << "=============================================" << endl;
			cout << "Exporting header and body messages..." << endl;
			exportReply(curl_handle);
			cout << "=============================================" << endl;
		}

		/* cleanup curl stuff */
		curl_easy_cleanup(curl_handle);

		return readBuffer;
	}

	double getNonce()
	{
		return time(NULL);
	}

	// ===========================================================
	//Public API
	// ===========================================================

	void setGetMarketSummary(string market)
	{
		ssURL.str("");
		ssURL << "https://bittrex.com/api/v1.1/public/getmarketsummary?market=" << market;
	}

	void setGetOrdersBook(const string market, const int depth)
	{
		ssURL.str("");
		ssURL << "https://bittrex.com/api/v1.1/public/getorderbook?market=" << market << "&type=both&depth=" << depth;
	}

	void setGetMarketHistory(string market, int depth)
	{
		ssURL.str("");
		ssURL << "https://bittrex.com/api/v1.1/public/getmarkethistory?market=" << market;
	}

	// ===========================================================
	//Market
	// ===========================================================

	void setBuyLimit(string market, double quantity, double rate)
	{
		ssURL.str("");
		ssURL << "https://bittrex.com/api/v1.1/market/buylimit?apikey=" << API_PUBLIC_KEY << "&nonce=" << getNonce() << "&market=" << market << "&quantity=" << quantity << "&rate=" << rate;

	}

	void setSellLimit(string market, double quantity, double rate)
	{
		ssURL.str("");
		ssURL << "https://bittrex.com/api/v1.1/market/selllimit?apikey=" << API_PUBLIC_KEY << "&nonce=" << getNonce() << "&market=" << market << "&quantity=" << quantity << "&rate=" << rate;

	}

	void setCancelOrder(string uuid)
	{
		ssURL.str("");
		ssURL << "https://bittrex.com/api/v1.1/market/cancel?apikey=" << API_PUBLIC_KEY << "&nonce=" << getNonce() << "&uuid=" << uuid;
	}

	void setVerifyMyOpenOrders(string market = "empty")
	{
		ssURL.str("");
		if (market != "empty")
			ssURL << "https://bittrex.com/api/v1.1/market/getopenorders?apikey=" << API_PUBLIC_KEY << "&nonce=" << getNonce() << "&market=" << market;
		else
			ssURL << "https://bittrex.com/api/v1.1/market/getopenorders?apikey=" << API_PUBLIC_KEY << "&nonce=" << getNonce();
	}

	// ===========================================================
	// ===========================================================

	// ===========================================================
	//Account Api
	// ===========================================================

	void setGetBalance(string currency = "empty")
	{
		ssURL.str("");
		if (currency != "empty")
			ssURL << "https://bittrex.com/api/v1.1/account/getbalances?apikey=" << API_PUBLIC_KEY << "&nonce=" << getNonce() << "&currency=" << currency;
		else
			ssURL << "https://bittrex.com/api/v1.1/account/getbalances?apikey=" << API_PUBLIC_KEY << "&nonce=" << getNonce();
	}

	//TODO
	//	ssURL << "https://bittrex.com/api/v1.1/account/getdepositaddress?apikey="
	//			<< apikey << "&nonce=" << nonce << "&currency=DOGE";

	// ===========================================================
	// ===========================================================
	//	Other useful functions
	// ===========================================================
	static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
	{
		size_t written = fwrite(ptr, size, nmemb, (FILE *) stream);
		return written;
	}

	void exportReply(CURL* curl_handle)
	{
		static const char *headerfilename = "headAnswer.out";
		FILE *headerfile;
		static const char *bodyfilename = "bodyAnswer.out";
		FILE *bodyfile;

		CURLcode res = curl_easy_perform(curl_handle);

		/* open the header file */
		headerfile = fopen(headerfilename, "wb");
		if (!headerfile)
		{
			curl_easy_cleanup(curl_handle);
			cout << "error on exporting header file!" << endl;
			exit(-1);
		}

		/* open the body file */
		bodyfile = fopen(bodyfilename, "wb");
		if (!bodyfile)
		{
			curl_easy_cleanup(curl_handle);
			fclose(headerfile);
			cout << "error on exporting body file!" << endl;
			exit(-1);

		}

		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

		/* we want the headers be written to this file handle */
		curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, headerfile);

		/* we want the body be written to this file handle instead of stdout */
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, bodyfile);

		/* get it! */
		curl_easy_perform(curl_handle);

		/* close the header file */
		fclose(headerfile);

		/* close the body file */
		fclose(bodyfile);
	}
	// ===========================================================
	// ===========================================================
};

#endif /*BITTREX_API_HPP*/
