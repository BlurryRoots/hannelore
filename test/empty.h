
// empty test scenario
SCENARIO () {
	GIVEN ("Empty scenario") {
		WHEN ("run") {
			THEN ("nothing should happen") {
				REQUIRE (true == true);
			}
		}
	}
}
