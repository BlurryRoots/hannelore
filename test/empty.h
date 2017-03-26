// empty test scenario
SCENARIO ("empty test") {
	GIVEN ("Empty scenario") {
		WHEN ("run") {
			THEN ("nothing should happen") {
				REQUIRE (true == true);
			}
		}
	}
}

#include "Hashing.h"
#include <string>
SCENARIO ("hashing") {
	GIVEN ("A certain string") {
		std::string str = "This is a simple test!";

		WHEN ("hashed with djb") {
			unsigned int hash = blurryroots::hashing::djb::hash (str);

			THEN ("nothing should happen") {
				REQUIRE (hash != 0);
			}
		}
	}
}