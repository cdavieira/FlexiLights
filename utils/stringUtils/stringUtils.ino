#include "stringUtils.hpp"
#include <FastLED.h>

extern CRGBPalette16 currentPalette;

uint8_t getLength(const char* const name){
	uint8_t count = 0;
	while(name[count] != '\0'){
		count++;
	}
	return count;
}

bool compareNames(const char* const name1, const char* const name2){
	const uint8_t name1_len = getLength(name1);
	const uint8_t name2_len = getLength(name2);
	const uint8_t max_chars = name1_len > name2_len? name2_len : name1_len;
	for(uint8_t i=0; i<max_chars; i++){
		if(name1[i] != name2[i]){
			return false;
		}
	}
	return true;
};

void ChoosePalleteByName(const char* const name){
	if(compareNames(name, "R1X")){
		currentPalette = RainbowColors_p;
	}
	else if(compareNames(name, "R2X")){
		currentPalette = RainbowStripeColors_p;
	}
	else if(compareNames(name, "O1X")){
		currentPalette = OceanColors_p;
	}
	else if(compareNames(name, "C1X")){
		currentPalette = CloudColors_p;
	}
	else if(compareNames(name, "L1X")){
		currentPalette = LavaColors_p;
	}
	else if(compareNames(name, "F1X")){
		currentPalette = ForestColors_p;
	}
	else if(compareNames(name, "P1X")){
		currentPalette = PartyColors_p;
	}
}

