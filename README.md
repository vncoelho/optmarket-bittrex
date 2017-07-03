optmarket-bittrex
==========

optmarket-bittrex is framework mainly designed for communicating with Bittrex API (public and private) in c++.
Some templates are available for transforming the adquired data into vectors, providing an insight for the use of other efficient data types.
We believe that optimization, metaheuristics and other intelligent inspired systems can be easily embedded inside it.
For advancing into these directions, please check the [OptFrame](https://sourceforge.net/projects/optframe/).

This version implement V1.1 Bittrex API and the new HMAC authentification.

## Import
git clone "github.com/vncoelho/optmarket-bittrex.git"
	
## Usage
~~~ c++

string apikey = "YOUR_API_KEY";
string apisecret = "YOUR_API_SECRET";

int main(void)
{
	// Bittrex main class
	BittrexAPI bittrex;
	
	// Optimization market functions with templates for automatic and expert training
	OptMarket optMarket;
	//Other -- Precise printing messages
	cout << std::setprecision(10);
	cout << std::fixed;

	//Few examples
	float quantity = 0.3000;
	float rate = 0.00313001;

	optMarket.optSellCalculatingProfit(bittrex,quantity,rate,"BTC-ANS");
	bittrex.setGetOrdersBook("BTC-ANS", 1);
	bittrex.setGetMarketSummary("BTC-ANS");
	bittrex.setGetBalance("ANS");
	bittrex.setCancelOrder("uuid");
	bittrex.setSellLimit("BTC-ANS", quantity, rate);
	bittrex.setBuyLimit("BTC-ETH", quantity, rate); //Min 50K * 10^-8
	bittrex.setGetBalance("ANS");
	bittrex.setVerifyMyOpenOrders("BTC-ANS");

	cout << "Finished with success!" << endl;
	return 0;
}
~~~	

Donate
------

## BTC
![Donation QR](http://i.imgur.com/xC5SnSL.png)

[17GrXw3qdz1G6DRxvtmhnhLzFXjRK5z6NY](http://i.imgur.com/xC5SnSL.png)

## ANS
AZZZsZJVqZwfHUx55p6UdY1Vx7qCjhnjMy

## ETH
0xd35fb76723636e11Fa580665A54E19F781C6Cd8a

