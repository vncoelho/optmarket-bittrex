#ifndef OPTMARKET_HPP
#define OPTMARKET_HPP

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>

#include "bittrexApi.hpp"

using namespace std;

struct currencyBalance
{
	float balance, available, pending;
	string market;
	currencyBalance(string _market = "", float _balance = 0, float _available = 0, float _pending = 0) :
			market(_market), balance(_balance), pending(_pending), available(_available)
	{

	}

};

template<class T>
ostream& operator<<(ostream &os, const vector<T> &obj)
{
	os << "vector(" << obj.size() << ") [";

	if (obj.size() > 0)
	{
		for (unsigned int i = 0; i < obj.size() - 1; i++)
			os << obj.at(i) << " , ";
		os << obj.at(obj.size() - 1);
	}

	os << "]";
	return os;
}

class OptMarket
{
private:

public:

	OptMarket()
	{

	}

	virtual ~OptMarket()
	{
	}

	currencyBalance getMarketBalanceFromAPI(string buffer, string market)
	{
//		cout << market << endl;
//		cout << buffer.find(market) << endl;
//		getchar();
		string cutUntilDesignedCurrencyBalance = buffer.substr(buffer.find(market));
//		cout << cutUntilDesignedCurrencyBalance << endl;
//		getchar();
		size_t pos = cutUntilDesignedCurrencyBalance.find(":");
		size_t posEnds = cutUntilDesignedCurrencyBalance.find("Available");
		string balance = cutUntilDesignedCurrencyBalance.substr(pos + 1, posEnds - pos - 3);
//		cout << "current balance of " << market << " is:" << balance << endl;
		currencyBalance cB(market, stod(balance), 0, 0);
		return cB;
	}


	void optSellCalculatingProfit(BittrexAPI& bittrex, float quantity, float rate, string market)
	{
		float theoricalTotal = (quantity * rate);
		float realTotal = theoricalTotal * (1 - bittrex.getFeesPerOrder());
		float costs = theoricalTotal - realTotal;
		float actualRate = realTotal / rate;
		cout << "Selling rate:" << rate << "\tq:" << quantity << "\ntTotal:" << theoricalTotal << "\trTotal:" << realTotal << "\tcosts:" << costs << "\taRate:" << actualRate << endl;

		string baseCurrency = market.substr(0,market.find("-"));
		string tradedCurrency = market.substr(market.find("-") + 1);

		bool print = true;
		bool sign = true; //If you want to sign
		bool exportReply = false;

		InstOptions instOpt(print,sign,exportReply);

		bittrex.setGetBalance(tradedCurrency);
		string buffer = bittrex.callCurlPlataform(InstOptions(false,true,false));
		currencyBalance cBT = getMarketBalanceFromAPI(buffer, tradedCurrency);
		currencyBalance cBB = getMarketBalanceFromAPI(buffer, baseCurrency);

		bittrex.setSellLimit(market, quantity, rate);
		buffer = bittrex.callCurlPlataform(InstOptions(true,true,false));

		//TODO - Create a function for checking pending orders and wait
		//Go to the multicore, multigpu and run all this in parallel

		bittrex.setGetBalance(tradedCurrency);
		buffer= bittrex.callCurlPlataform(InstOptions(false,true,false));
		currencyBalance cBTNew = getMarketBalanceFromAPI(buffer, tradedCurrency);
		currencyBalance cBBNew = getMarketBalanceFromAPI(buffer, baseCurrency);

		cout<<"Traded(Old,New):("<< cBT.balance<<","<<cBTNew.balance<<")"<<endl;
		cout<<"Base(Old,New):("<< cBB.balance<<","<<cBBNew.balance<<")"<<endl;
		cout << "Sold value of traded is: " << cBTNew.balance - cBT.balance << endl;
		cout << "Amount of coins bought for the base currency is: " << cBBNew.balance - cBB.balance << endl;
		getchar();

	}

	vector<vector<double> > transformBookToVectors(string buffer)
	{
		vector<vector<double> > buyAndSellOrders;

		size_t pos = buffer.find("sell");
		string bufferBuy = buffer.substr(0, pos);
		string bufferSell = buffer.substr(pos);

		//one iteration for buying and another one for selling
		for (int i = 0; i < 2; i++)
		{
			vector<double> vQuantity;
			vector<double> vRate;

			string bufferToTransform;
			if (i == 0)
				bufferToTransform = bufferBuy;
			else
				bufferToTransform = bufferSell;

			//Cutting vector until Quantity begins
			pos = bufferToTransform.find("Quantity");
			bufferToTransform = bufferToTransform.substr(pos);

			size_t posStartValue = 0, quantityValuePosEnds = 0, posRateValueEnds = 0;
			while (posRateValueEnds < bufferToTransform.size())
			{
				posStartValue = bufferToTransform.find(":");
				quantityValuePosEnds = bufferToTransform.find(",");
				//		cout<<bufferBuy<<endl;
				//		getchar();

				string quantityValue = bufferToTransform.substr(posStartValue + 1, quantityValuePosEnds - posStartValue - 1);
				posRateValueEnds = bufferToTransform.find("}");
				string rateValue = bufferToTransform.substr(quantityValuePosEnds + 8, posRateValueEnds - quantityValuePosEnds - 8);

				//		string quantityValue= bufferBuy.substr (pos+10,11);
				// + 5 is just a number of pushing to the next value
				cout << "\n\n extracted value is:" << endl;
				cout << "quantityValue\t" << quantityValue << endl;
				cout << "rateValue\t" << rateValue << endl;
				double quantity = stod(quantityValue);
				double rate = stod(rateValue);

				vQuantity.push_back(quantity);
				vRate.push_back(rate);

				bufferToTransform = bufferToTransform.substr(posRateValueEnds + 3);
			}

			buyAndSellOrders.push_back(vQuantity);
			buyAndSellOrders.push_back(vRate);

			cout << buyAndSellOrders << endl;
			cout<<"\npress any key to continue"<<endl;
			getchar();
		}

		cout<<"\nConverted to vector! press any key to continue"<<endl;
		getchar();

		return buyAndSellOrders;
	}

};

#endif /*OPTMARKET_HPP*/
