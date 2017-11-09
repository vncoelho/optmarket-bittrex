#include <stdlib.h>     /* getenv */
#include <unistd.h> /* sleep */
#include <sstream>

#include "bittrexApi.hpp"
#include "optMarket.hpp"

using namespace std;

string API_PUBLIC_KEY, API_PRIVATE_KEY;

void readKeys(string& apikey, string& apisecret)
{
	apikey = getenv("PUB_KEY");
	apisecret = getenv("PRIVATE_KEY");

	//==================================================
	//Deprecated strategy for getting keys from a local file
	//	string str;
	//	ifstream t("keys.input");
	//	getline(t, str);
	//	getline(t, apikey);
	//	getline(t, str);
	//	getline(t, apisecret);
	//==================================================

//	cout << apikey << endl;
//	cout << apisecret << endl;
//	cout << getenv("PUB_KEY") << endl;
//	cout << getenv("PRIVATE_KEY") << endl;
//	getchar();
}

int main(void)
{
	//Reading apikey and secret located at keys.input
	readKeys(API_PUBLIC_KEY, API_PRIVATE_KEY);

	// Bittrex main class
	BittrexAPI bittrex;
	InstuctionOptions instOpt;
	string output_buffer;

	// Optimization market functions with templates for automatic and expert training
	OptMarket optMarket;
	//Other -- Precise printing messages
	cout << std::setprecision(10);
	cout << std::fixed;

	double quantity = 5;
	double rate = 0.00012;

//	bittrex.setSellLimit("BTC-TRIG", quantity, rate);
//	bittrex.setCancelOrder("aa408c07-3d7f-4696-8102-f8fb9b5ed2be");
	bool print, sign, exportReply;

	optMarket.optGetBalanceBittrex(bittrex, "TRIG");

//	getchar();

	instOpt = InstuctionOptions(print = false, sign = true, exportReply = false);
//	output_buffer = bittrex.callCurlPlataform(instOpt);

//	optMarket.optSellCalculatingProfit(bittrex,quantity,rate,"BTC-NEO");
	optMarket.callBookOffers_ToTimeSeries_PlusAutomaticActions(bittrex, "USDT-BTC", 50);

	cout << "main.cpp::Waiting!" << endl;
	getchar();
//
	optMarket.optGetBalanceBittrex(bittrex, "NEO");

	optMarket.optGetBalanceBittrex(bittrex, "NEO");

	bittrex.setGetOrdersBook("BTC-NEO", 1);
	bittrex.setGetMarketSummary("BTC-NEO");
	bittrex.setGetBalance("NEO");
	bittrex.setCancelOrder("uuid");
	bittrex.setSellLimit("BTC-NEO", quantity, rate);
	bittrex.setBuyLimit("BTC-NEO", quantity, rate); //Min 50K * 10^-8
	bittrex.setVerifyMyOpenOrders("BTC-NEO");
	bittrex.setGetOrdersBook("BTC-NEO", 1);
	bittrex.setGetBalance("ANS");
	bittrex.setGetOrdersBook("BTC-NEO", 1);
	bittrex.setGetBalance("NEO");
	bittrex.setGetOrdersBook("BTC-ETH", 1);
	bittrex.setGetMarketSummary("BTC-NEO");

	bittrex.setGetBalance("BTC");

	instOpt = InstuctionOptions(print = true, sign = true, exportReply = false);
	//Executing instruction
	output_buffer = bittrex.callCurlPlataform(instOpt);

//	InstuctionOptions instOpt(print, sign, exportReply);
	//Executing instruction
//	string buffer = bittrex.callCurlPlataform(instOpt);

//	//Example for putting all orders into vectors, quantities and rates
//	vector<vector<double> > orderBookVectors = optMarket.transformBookToVectors(buffer);

	cout << "Finished with success!!" << endl;
	return 0;
}
