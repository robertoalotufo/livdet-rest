#include <iostream>
#include <fstream>
#include <cpr/cpr.h>
#include "docopt.h"
#include "json11.hpp"

using namespace std;
using namespace json11;

static const char _USAGE[] =
    R"(Livdet Predictor.

Usage:
  __PROGNAME__ -i img [--csv-out file]
  __PROGNAME__ (-h | --help)
  __PROGNAME__ --version

Options:
  -h --help       Show this screen.
  --version       Show version.
  -i img          Input image file.
  --csv-out file  Put the results in a csv file (separated with ';')
)";

/*
 * Search for a substring and replace all instances
 */
void search_and_replace(std::string& str, const char *old_str, const char *new_str)
{
	std::string::size_type index = 0;
	std::string::size_type old_str_len = strlen(old_str);
	std::string::size_type new_str_len = strlen(new_str);
	while ((index = str.find(old_str, index)) != std::string::npos)
	{
		str.replace(index, old_str_len, new_str);
		index += new_str_len;
	}
}

int main(int argc, char** argv)
{
	std::string USAGE = _USAGE;

	/*
	 * Replace "__PROGNAME__" with the real program name
	 */
	search_and_replace(USAGE, "__PROGNAME__", argv[0]);

	std::map<std::string, docopt::value> args
		= docopt::docopt(USAGE,
						 { argv + 1, argv + argc },
						 true,               // show help if requested
						 "Livdet Client Predictor V0.1");  // version string
	bool write_csv = false;
	std::vector<string> img_file_list;
	std::ofstream csv_file;

	/*
	 * Check if the -i flag was used
	 */
	if (args["-i"])
	{
		img_file_list.push_back(args["-i"].asString().c_str());
	}

	/*
	 * Check if the --csv-out flag was used
	 */
	if (args["--csv-out"])
	{
		csv_file.open(args["--csv-out"].asString(), std::ofstream::out | std::ofstream::binary);
		csv_file << "\"Image File\";\"Genuine prob\";\"Fake prob\";\"Predict time\"\r\n";
		write_csv = true;
	}

	for (auto img_file : img_file_list)
	{
		/*
		 * Do a http Post operation
		 */
		auto r = cpr::Post(cpr::Url{"http://nm-livdet.ddns.net/api/predict/"},
						   cpr::Multipart{{"image", cpr::File{img_file}}});

		/*
		 * If the response isn't 201, something is wrong.
		 */
		if (r.status_code != 201)
		{
			cerr << "Unexpected response: " << r.status_code;
			return 1;
		}

		std::string json_err;

		/*
		 * Parse the Json response
		 */
		Json resp = Json::parse(r.text, json_err);

		/*
		 * If some error occurred when parsing the json string, print
		 * the error and exit
		 */
		if (json_err.length() > 0)
		{
			cerr << json_err << endl;
			return 1;
		}

		/*
		 * Extract the prediction data from the json object
		 */
		double result = resp["prediction"].number_value();
		string predict_time = resp["execution_time"].string_value();

		/*
		 * Output a csv file with the predictions if requested
		 */
		if (write_csv)
		{
			csv_file << "\"" << img_file << "\";" << result << ";" << (1 - result) << ";" << predict_time << "\r\n";
		}

		/*
		 * Print results
		 */
		cout << "Image: " << img_file << endl;
		cout << "genuine: " << result << endl;
		cout << "fake: " << (1 - result) << endl;
		cout << "Prediction time: " << predict_time << endl;
	}

	/*
	 * Close the CSV file
	 */
	if (write_csv)
	{
		csv_file.close();
	}
	return 0;
}
