#pragma once
#include "../src/include/BaseView.h"
#include "../src/include/HttpResponse.h"
#include "../src/include/HttpServer.h"
#include <iostream>

void TEST_DB()
{
	SimpleDB testDB(BASE_DIR + "test/test.db");
	testDB.write({"table_1", "some_data_1"}, true);
	testDB.write({"table_2", "some_data_2"});
	testDB.write({"table_1", "some_data_3"}, true);
	testDB.write({"table_2", "some_data_4"});
	testDB.write({"table_2", "some_data_4"}, true);
}
