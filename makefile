all: test
	

CPPFLAGS = -stdlib=libc++ 
LDFLAGS = -lcpprest -lssl -lcrypto -lboost_system -lboost_thread-mt -lboost_chrono-mt
#-ljsoncpp 
#-Wall 
test:
	g++ -Ofast -std=c++11 main.cpp -ljsoncpp -lpthread -ldl -lcurl -lcrypto -o app_OptMarket
	#g++ $(GCC_FLAGS) $(RESTSDK_FGLAS) ./main.cpp -o ./testAquiVamos
	#g++  -std=c++11 main.cpp -o my_file -lcpprest -lssl -lcrypto -lboost_system
	#g++ main.cpp -ljsoncpp -lcurl -ljsonrpccpp-common -ljsonrpccpp-client -o sampleclient


clean:
	#make clean -C ./MyProjects/
	
