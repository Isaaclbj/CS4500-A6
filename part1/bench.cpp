#include "modified_dataframe.h"
#include <fstream>
#include <chrono>
#include <iostream>
#include <stdlib.h>
#include "string.h"
#include <string>
#include <math.h>

//simple adding function
class Adder : public Rower
{
public:
	int total;
	int count;

	Adder()
	{
		total = 0;
		count = 0;
	}

	bool accept(Row& r)
	{
		std::cout << "Rower1: " << count << std::endl;


		total += r.get_int(0) + r.get_int(1) + r.get_int(2)
			+ r.get_int(3) + r.get_int(4);
	}

	void join_delete(Rower* other)
	{
		total += dynamic_cast<Adder*>(other)->total;
		delete other;
	}
};

//expensive trig functions
class Trigger : public Rower
{
public:
	double total;
	int count;
	Trigger()
	{
		total = 10;
		count = 0;
	}

	bool accept(Row& r)
	{
		std::cout << "Rower2: " << count << std::endl;

		total += (r.get_bool(1));

		//Cases where true for last 3
		if (!r.get_bool(5) && r.get_bool(6) && r.get_bool(7)
			&& r.get_bool(8))
		{
			total = sqrt(abs(total));
		}
		//Cases where true for first 3
		else if(r.get_bool(5) && r.get_bool(6) && r.get_bool(7)
			&& !r.get_bool(8))
		{
			total = cos(total);
		}
		//Cases where true for first 2
		else if(r.get_bool(5) && r.get_bool(6) && !r.get_bool(7)
			&& !r.get_bool(8))
		{
			total = sin(total);
		}
	}

	void join_delete(Rower* other)
	{
		total += dynamic_cast<Trigger*>(other)->total;
		delete other;
	}
};

int main(int argc, char **argv)
{
	const char* FILENAME = "modified_datafile.txt";
	//open datafile.txt
	std::ifstream datafile(FILENAME);

	std::string line;

	Schema sch("IIIIIBBBBS");
	DataFrame df(sch);

	//read in data to dataframe
	while (datafile.peek() != EOF)
	{
      getline(datafile, line);
	  std::cout << "LINE IS " << line << std::endl;
	  //tokenize by using space as delimiter
	  char buff[line.length() + 1];
	  strcpy(buff, line.c_str());

	  char * temp = strtok(buff, " ");
	  std::cout << "TEMP IS " << temp << std::endl;
	  Row r(df.get_schema());
	  //we know there's only 10 items in each row
	  for (int i = 0; i < 10; i++)
	  {
		  std::cout << "Adding " << temp << " to row at index " << i << std::endl;
		  //feed tokens into row
		  if (i <= 4)
		  {
			  std::cout << "BREAKING HERE?" << std::endl;
			  r.set(i, (int)atoi(temp));
			  std::cout << "DOESN'T GET HERE" << std::endl;
		  }
		  else if (i <= 8)
		  {
			  r.set(i, (bool) atoi(temp));
		  }
		  else if (i = 9)
		  {
			  String* s = new String(temp, strlen(temp));
			  r.set(i, s);
		  }
	  }

	  df.add_row(r);

	}
	std::cout << "FINISHED POPULATING DATAFRAME!" << std::endl;
	datafile.close();

	return 0;
}