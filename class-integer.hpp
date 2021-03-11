
#include <iostream>
using namespace std;
class Integer {
private:
	int data;
public:
	Integer(const int &value) : data(value) {}
	Integer(const Integer &other) : data(other.data) {}
	bool operator==(const Integer &t)
	{
		return data == t.data;
	}
    friend ostream &operator<<( ostream &output,
                                const Integer &D )
    {
        output << D.data;
        return output;
    }
};
