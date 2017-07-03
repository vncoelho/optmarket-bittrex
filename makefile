all: optMarket
	
#-Wall 
optMarket:
	g++ -std=c++11 main.cpp -ljsoncpp -lcurl -lcrypto -o optMarket 
	
clean:
	#make clean -C ./MyProjects/
	
