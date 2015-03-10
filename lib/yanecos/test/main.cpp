#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <yanecos/EntityManager.h>

SCENARIO () {
	GIVEN ("An entitie manager") {
		Yanecos::EntityManager entities;

		WHEN ("query for all entities is made") {
			auto result = entities.get_all_entities ();
			THEN ("the number of entities in the result should be zero") {
				REQUIRE (0 == result.size ());
			}
		}
	}
}
