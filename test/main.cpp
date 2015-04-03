#define CATCH_CONFIG_MAIN
#include <catch.hpp>

SCENARIO () {
	GIVEN ("Empty scenario") {
		WHEN ("run") {
			THEN ("nothing should happen") {
				REQUIRE (true == true);
			}
		}
	}
}
