// C++ program to convert hexadecimal
// string to ASCII format string
#include <bits/stdc++.h>
using namespace std;

string hexToASCII(string hex)
{
	// initialize the ASCII code string as empty.
	string ascii = "";
	for (size_t i = 0; i < hex.length(); i += 2)
	{
		// extract two characters from hex string
		string part = hex.substr(i, 2);

		// change it into base 16 and
		// typecast as the character
		char ch = stoul(part, nullptr, 16);

		// add this char to final ASCII string
		ascii += ch;
	}
	return ascii;
}

// Driver Code
int main()
{
	// print the ASCII string.
	cout << hexToASCII("676565") << endl;

	return 0;
}

// This code is contributed by
// sanjeev2552

