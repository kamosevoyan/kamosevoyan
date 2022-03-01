#include <iostream>
#include <string>
#include <cassert>
#include <algorithm>

class HugeInt
{
	friend std::ostream& operator<<(std::ostream&, const HugeInt&);
	friend const HugeInt abs(const HugeInt&);

	public:
		HugeInt(const std::string&);
		HugeInt(int);

		const HugeInt operator+(const HugeInt&) const;
		const HugeInt operator*(const HugeInt&) const;
		const HugeInt operator/(const HugeInt&) const;
		const HugeInt operator-(const HugeInt&) const;
		const HugeInt operator%(const HugeInt&) const;

		const HugeInt operator-() const;

		bool operator>(const HugeInt&) const;
		bool operator<=(const HugeInt&) const;
		bool operator<(const HugeInt&) const;
		bool operator>=(const HugeInt&) const;
		bool operator==(const HugeInt&) const;
		bool operator!=(const HugeInt&) const;

		void operator+=(const HugeInt& other);
		void operator-=(const HugeInt& other);
		void operator*=(const HugeInt& other);
		
		HugeInt& operator++();
		HugeInt& operator--();

		const HugeInt operator++(int);
		const HugeInt operator--(int);

		HugeInt(HugeInt&&);
		HugeInt(const HugeInt&);
		HugeInt& operator=(const HugeInt&);

	private:
		std::string data;
		int len = 0;
		int sign;

};


HugeInt::HugeInt(HugeInt&& other)
{
	this->data = std::move(other.data);
	this->len = other.len;
	this->sign = other.sign;
}


HugeInt::HugeInt(const HugeInt& other)
{
	this->data = other.data;
	this->len = other.len;
	this->sign = other.sign;
}


HugeInt& HugeInt::operator=(const HugeInt& other)
{
	this->data = other.data;
	this->len = other.len;
	this->sign = other.sign;
	return *this;
}

const HugeInt HugeInt::operator--(int)
{
	HugeInt temp = *this;

	(*this) += 1;

	return temp;
}

const HugeInt HugeInt::operator++(int)
{
	HugeInt temp = *this;

	(*this) -= 1;

	return temp;
}

HugeInt& HugeInt::operator++()
{
	(*this) += 1;
	return *this;
}

HugeInt& HugeInt::operator--()
{
	(*this) -= 1;
	return *this;
}

const HugeInt HugeInt::operator%(const HugeInt& other) const
{
	if (this == 0)
	{
		if (other != 0)
			return other;
		else 
			assert(0 != 0);
	}

	if (other == 1)
		return 0;

	return (*this) - (*this) / other * other;
}

void HugeInt::operator+=(const HugeInt& other)
{
	*this = *this + other;
	return;
}

void HugeInt::operator-=(const HugeInt& other)
{
	*this = *this - other;
	return;
}

void HugeInt::operator*=(const HugeInt& other)
{
	*this = *this * other;
	return;
}

const HugeInt abs(const HugeInt& hugeint) 
{
	HugeInt temp(hugeint);
	temp.sign = 1;

	return temp;
}

const HugeInt HugeInt::operator-() const
{
	HugeInt out(*this); 

	out.sign *= -1;

	return out;
}

const HugeInt HugeInt::operator/(const HugeInt& _other) const
{
	if (*this == 0)
	{
		if (_other != 0)
			return 0;
		assert(0 != 0);
	}

	if (_other == 1)
		return *this;

	if (_other == -1)
		return (-(*this));

	
	if (abs(*this) == abs(_other))
		return HugeInt(this->sign *_other.sign);
	
	HugeInt out(0);

	HugeInt div( abs(*this));

	HugeInt other = abs(_other);

	if (div < other)
		return 0;
	
	while (div >= other)
	{
		HugeInt part(0);
		part.data.erase();
	 
		HugeInt temp(0);
		temp.data.erase();
		
		int count = 0;	
		int shift = 0;
		int div_len = div.len;
		
		if (div.data[0] == '0')
		{
			div.data.erase(0,1);
			--div.len;
			continue;
		}
		
		do	
		{
			part.data.push_back(div.data[0]);
			div.data.erase(0,1);
			--div.len;
			++part.len;
		}
		while (part < other);
		
		shift = div_len - part.len;
		
		while (part >= other)
		{
			part -=  other;
			++count;
		}
				
		if (part != 0)
		{
			std::reverse(part.data.begin(), part.data.end());
			std::reverse(div.data.begin(), div.data.end());


			while (part.len > 0)
			{
				div.data.push_back(part.data[0]);
				++div.len;
				part.data.erase(0,1);
				--part.len;
			}

			std::reverse(div.data.begin(), div.data.end());
		}
			
		temp.data.push_back(count + '0');
		++temp.len;
		
		for (int i = 0; i < shift; ++i)
		{
			temp.data.push_back('0');
			++temp.len;
		}
		
		out+=temp;		

	}

	out.sign = this->sign * _other.sign;

	return out;
}


const HugeInt HugeInt::operator-(const HugeInt& other) const
{
	return *this + (-other);
}

bool HugeInt::operator>=(const HugeInt& other) const
{
	return !(*this < other);
}

bool HugeInt::operator<(const HugeInt& other) const
{
	return (*this <= other) && (*this != other);
}

bool HugeInt::operator<=(const HugeInt& other) const
{
	return !(*this > other);
}

bool HugeInt::operator!=(const HugeInt& other) const
{
	return !(*this == other);
}

bool HugeInt::operator==(const HugeInt& other) const
{
	if (this->sign != other.sign)
		return false;

	if (this->len != other.len)
		return false;

	bool state = true;
	int index = 0;

	while(index < this->len)
	{
		if (this->data[index] != other.data[index])
		{
			state = false;
			break;
		}
		++index;
	}
	return state;


}

bool HugeInt::operator>(const HugeInt& other) const
{
	if (this->sign != other.sign)
		return this->sign == 1 ? true : false;
	else
	{
		if (this->len > other.len)
			return (this->sign == 1) && true;

		if (this->len < other.len)
			return (this->sign == -1) && true;

		int index = 0;
		bool state = true;

		while(index < this->len)
		{
			if (this->data[index] > other.data[index])
				return true;

			if (this->data[index] < other.data[index])
				return false;

			++index;
		}
		return false;
		
	}
}

std::ostream& operator<<(std::ostream& out, const HugeInt& hugeint)
{
	out<<(hugeint.sign == -1 ? "-":"")<<hugeint.data;
	return out;
}

HugeInt::HugeInt(const std::string& str)
{

	if (str.length() == 0)
		assert(0 != 0);

	if (str == "0")
	{
		this->data = str;
		this->len = 0;
		this->sign = 1;
		return;
	}

	if (str[0] == '-')
		this->sign = -1;
	else
		this->sign = 1;
	
	for (int i = 0; i < str.length(); i++)
		if (std::isdigit(str[i]) || (str[i] == '-' && i == 0));
			else
				assert(0 != 0);
	
	this->data = str;

	if (this->sign == -1)
	       this->data.erase(0, 1);	

	this->len = this->data.length();


}

HugeInt::HugeInt(int digit) : HugeInt(std::to_string(digit))
{

}


const HugeInt HugeInt::operator+(const HugeInt& other) const
{
	if (*this == 0)
		return other;

	if (other == 0)
		return *this;

	std::string output;
	int min = this->len <= other.len ? this->len : other.len;
	int max = this->len >= other.len ? this->len : other.len;
	int carry = 0;
	int sign;
	int raw_sum;
	const HugeInt* MAX = this->len == max ? this : &other;


	for (int index = 0; index < min; ++index)
	{
		raw_sum = (((this->data[this->len - 1 - index] + carry - '0') < (other.data[other.len - 1 - index] - '0') && (this->sign != other.sign) ? 1 : 0) * 10 + this->sign * (this->data[this->len - 1 - index] - '0') + other.sign * (other.data[other.len - 1 - index] - '0'))  + carry;
		output.push_back(abs(raw_sum) % 10 + '0');
		carry = raw_sum >= 10 ? 1 : (raw_sum <= -10 || ((this->data[this->len - 1 - index] + carry  - '0') < (other.data[other.len - 1 - index] - '0') ) && (this->sign != other.sign) ? -1 : 0);
		sign = raw_sum >= 0 ? 1 : -1;

	}

	for (int index = max - min - 1; index >= 0; --index)
	{
		raw_sum = MAX->data[index] - '0' + carry;
		output.push_back( raw_sum % 10 + '0');
		carry = raw_sum >= 10 ? 1 : (raw_sum <= -10 ? -1 : 0);	
	}
	

	if (carry)
		output.push_back('1');	

	std::reverse(output.begin(), output.end());

	HugeInt out(output);
	out.sign = sign;

	//Erasing odd zeros
	
	while (out.data[0] == '0' and out.len > 1)
	{
		out.data.erase(0,1);
		--out.len;
	}

	return out;
}

const HugeInt HugeInt::operator*(const HugeInt& other) const
{

	if (*this == 0 || other == 0)
		return 0; 
	
	if (*this == 1)
		return other;
	if (*this == -1)
		return (-other);
	if (other == 1)
		return *this;
	if (other == -1)
		return -(*this);

	HugeInt output(0);
	
	HugeInt p1(abs(*this)),p2(abs(other));

	const HugeInt* max_ptr = this->len >= other.len ? &p1 : &p2;
	const HugeInt* min_ptr = this->len <  other.len ? &p1 : &p2;

	for (int i = min_ptr->len - 1; i >= 0; --i)
	{
		if (min_ptr->data[i] == '0')
			continue;

		HugeInt temp(0);
		

		for (int j = 0; j < min_ptr->data[i] - '0'; ++j)
			temp += *max_ptr;

		for (int k = 1; k < min_ptr->len - i; ++k)
		{
			temp.data.push_back('0');
			++temp.len;
		}
			
		output += temp;

	}

	output.sign = this->sign * other.sign;

	return output;
}


int main()
{
		
	HugeInt a("-11111111111119287398273982792732973298729371111111111111111111111111111111111111111111111111111111111111111111111111111111"),b("-7777777777777777777777770293802983083092809238092830823982323882398273987377777777");
	//Test: should return 0

	std::cout<<(a * b / a -b )<<std::endl;

}
