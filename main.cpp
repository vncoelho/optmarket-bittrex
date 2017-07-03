#include "bittrexApi.hpp"
#include "optMarket.hpp"

using namespace std;

string apikey = "";
string apisecret = "";

int main(void)
{
	// Bittrex main class
	BittrexAPI bittrex;

	// Optimization market functions with templates for automatic and expert training
	OptMarket optMarket;
	//Other -- Precise printing messages
	cout << std::setprecision(10);
	cout << std::fixed;

	float quantity = 0.3000;
	float rate = 0.00313001;
//	optMarket.optSellCalculatingProfit(bittrex,quantity,rate,"BTC-ANS");

	bittrex.setGetOrdersBook("BTC-ANS", 1);
	bittrex.setGetMarketSummary("BTC-ANS");
	bittrex.setGetBalance("ANS");
	bittrex.setCancelOrder("uuid");
	bittrex.setSellLimit("BTC-ANS", quantity, rate);
	bittrex.setBuyLimit("BTC-ANS", quantity, rate); //Min 50K * 10^-8
	bittrex.setVerifyMyOpenOrders("BTC-ANS");
	bittrex.setGetOrdersBook("BTC-ANS", 1);
	bittrex.setGetBalance("ANS");

	bool print = true;
	bool sign = true;
	bool exportReply = false;

	InstOptions instOpt(print, sign, exportReply);
	//Executing instruction
	string buffer = bittrex.callCurlPlataform(instOpt);

	//Example for putting all orders into vectors, quantities and rates
//	vector<vector<double> > orderBookVectors = optMarket.transformBookToVectors(buffer);
//
//	cout << "\nFinal answer in terms of vectors:" << endl;
//	cout << orderBookVectors << endl;

	cout << "Finished with success!" << endl;
	return 0;
}
