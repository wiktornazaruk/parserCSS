#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <stdio.h>
using namespace std;
#define T 8
#define LINE_SIZE 128
#define NUL '\0'

class str
{
public:
	static const size_t MAX_LENGTH = 127;
	char data[MAX_LENGTH + 1];
	str(const char* s = "")
	{
		size_t length = strlen(s);
		if (length > MAX_LENGTH)
		{
			length = MAX_LENGTH;
		}
		strncpy(data, s, length);
		data[length] = NUL;
	}
	str(const str& other)
	{
		strncpy(data, other.data, MAX_LENGTH);
		data[MAX_LENGTH] = NUL;
	}
	~str() {}
	size_t length() const
	{
		return strlen(data);
	}
	const char* getData() const
	{
		return data;
	}
	str& operator=(const str& other)
	{
		if (this != &other)
		{
			strncpy(data, other.data, MAX_LENGTH);
			data[MAX_LENGTH] = NUL;
		}
		return *this;
	}
	str& operator=(const char* s)
	{
		size_t length = strlen(s);
		if (length > MAX_LENGTH)
		{
			length = MAX_LENGTH;
		}
		strncpy(data, s, length);
		data[length] = NUL;
		return *this;
	}
	str operator+(const str& other) const
	{
		str result(*this);
		result += other;
		return result;
	}
	str operator+(const char* s) const
	{
		str result(*this);
		result += s;
		return result;
	}
	str& operator+=(const str& other)
	{
		size_t currentLength = strlen(data);
		size_t maxLength = MAX_LENGTH - currentLength;
		size_t otherLength = strlen(other.data);
		if (otherLength > maxLength)
		{
			otherLength = maxLength;
			cerr << "Error: string too long!" << endl;
		}
		strncat(data, other.data, otherLength);
		return *this;
	}
	str& operator+=(const char* s)
	{
		size_t currentLength = strlen(data);
		size_t maxLength = MAX_LENGTH - currentLength;
		size_t sLength = strlen(s);
		if (sLength > maxLength)
		{
			sLength = maxLength;
		}
		strncat(data, s, sLength);
		return *this;
	}
	str& operator+=(char c)
	{
		size_t length = strlen(data);
		if (length < MAX_LENGTH - 1)
		{
			data[length] = c;
			data[length + 1] = NUL;
		}
		return *this;
	}
	bool operator==(const str& other) const
	{
		return strcmp(data, other.data) == 0;
	}
	bool operator==(const char* s) const
	{
		return strcmp(data, s) == 0;
	}
	bool operator!=(const str& other) const
	{
		return !(*this == other);
	}
	bool operator!=(const char* s) const
	{
		return !(*this == s);
	}
	char& operator[](size_t index)
	{
		if (index >= strlen(data))
		{
			static char dummy = NUL;
			return dummy;
		}
		return data[index];
	}
	friend ostream& operator<<(ostream& os, const str& s)
	{
		os << s.data;
		return os;
	}
	friend istream& operator>>(std::istream& is, str& s)
	{
		char temp[MAX_LENGTH + 1];
		is.getline(temp, MAX_LENGTH + 1);
		s = temp;
		return is;
	}
};

struct Selector
{
public:
	str name = "";
	Selector* next = nullptr;
	Selector* prev = nullptr;
};

struct Attribute
{
public:
	str name = "";
	str value = "";
	Attribute* next = nullptr;
	Attribute* prev = nullptr;
};

struct Block
{
public:
	Selector* selector = nullptr;
	Attribute* attribute = nullptr;
	int num_of_selectors = 0;
	int num_of_attributes = 0;
};

struct Node
{
public:
	Block blocks[T];
	bool isOccupied[T] = { false };
	int realCounter = 0;
	int exculdedDeletedCounter = 0;
	Node* prev = nullptr;
	Node* next = nullptr;
};

Selector* newSelector(const str& selector_name)
{
	Selector* selector = new Selector;
	selector->name = selector_name;
	selector->next = NULL;
	return selector;
}

Selector* insertSelector(Selector* head, const str& selector_name)
{
	Selector* new_selector = newSelector(selector_name);
	new_selector->next = head;
	new_selector->prev = NULL;
	if (head != NULL)
	{
		head->prev = new_selector;
	}
	head = new_selector;
	return head;
}

Attribute* newAttribute(const str& attribute_name, const str& attribute_value)
{
	Attribute* attribute = new Attribute;
	attribute->name = attribute_name;
	attribute->value = attribute_value;
	attribute->next = NULL;
	return attribute;
}

Attribute* insertAttribute(Attribute* head, const str& attribute_name, const str& attribute_value)
{
	Attribute* new_attribute = newAttribute(attribute_name, attribute_value);
	new_attribute->next = head;
	new_attribute->prev = NULL;
	if (head != NULL)
	{
		head->prev = new_attribute;
	}
	head = new_attribute;
	return head;
}

void insertNode(Node** head)
{
	Node* new_node = new Node;
	new_node->next = *head;
	new_node->prev = NULL;
	if ((*head) != NULL)
	{
		(*head)->prev = new_node;
	}
	(*head) = new_node;
}

void deleteSelectors(int block_index, Node* node)
{
	Selector* head = node->blocks[block_index].selector;
	while (head != NULL)
	{
		Selector* temp = head;
		head = head->next;
		delete temp;
	}
	node->blocks[block_index].selector = NULL;
}

void deleteAttributes(int block_index, Node* node)
{
	Attribute* head = node->blocks[block_index].attribute;
	while (head != NULL)
	{
		Attribute* temp = head;
		head = head->next;
		delete temp;
	}
	node->blocks[block_index].attribute = NULL;
}

void deleteBlock(int block_index, Node* node)
{
	if (block_index < 0)
	{
		return;
	}
	else if (block_index >= T)
	{
		deleteBlock(block_index - T, node->next);
	}
	else
	{
		deleteSelectors(block_index, node);
		node->blocks[block_index].num_of_selectors = 0;
		deleteAttributes(block_index, node);
		node->blocks[block_index].num_of_attributes = 0;
		node->isOccupied[block_index] = false;
		node->realCounter--;
	}
}

void deleteNode(Node* node)
{
	for (int i = 0; i < T; i++)
	{
		if (node->isOccupied[i])
		{
			deleteBlock(i, node);
			node->isOccupied[i] = false;
		}
	}
	node->realCounter = 0;
	node->exculdedDeletedCounter = 0;
	delete node;
}

void deleteAllNodes(Node* node)
{
	while (node != NULL)
	{
		Node* temp = node;
		node = node->next;
		deleteNode(temp);
	}
}

struct variables
{
	bool isSelector = true;
	bool isKey = false;
	bool isValue = false;
	bool isCommand = false;
	bool ommitFirstSpace = false;
	int num_of_css_sections = 0;
	str key = "";
	str value = "";
	str selector_name = "";
};

Attribute* deleteAttribute(Attribute*& head_ref, Attribute* del)
{


	if (head_ref == NULL || del == NULL)
		return nullptr;

	if (head_ref == del)
		head_ref = del->next;

	if (del->next != NULL)
		del->next->prev = del->prev;

	if (del->prev != NULL)
		del->prev->next = del->next;

	delete del;
	return del;
}

void deleteDuplicateAttribute(Attribute* head, const str& attribute_name, int& num_of_attributes)
{
	Attribute* temp = head;
	while (head != NULL)
	{
		if (head->name == attribute_name)
		{
			deleteAttribute(temp, head);
			num_of_attributes--;
			return;
		}
		head = head->next;
	}
	return;
}

void parseCssLine(str& line, Node** node, variables& var)
{
	if (line == "????")
	{
		var.isCommand = true;
		return;
	}
	for (unsigned int i = 0; i < line.length(); i++)
	{
		switch (line[i])
		{
		case '{':
			(*node)->blocks[(*node)->exculdedDeletedCounter % T].selector = insertSelector((*node)->blocks[(*node)->exculdedDeletedCounter % T].selector, var.selector_name);
			(*node)->blocks[(*node)->exculdedDeletedCounter % T].num_of_selectors++;
			var.selector_name = "";
			var.isSelector = false;
			var.isKey = true;
			break;
		case '}':
			var.isSelector = true;
			var.isKey = false;
			(*node)->isOccupied[(*node)->exculdedDeletedCounter % T] = true;
			var.num_of_css_sections++;
			(*node)->realCounter++;
			(*node)->exculdedDeletedCounter++;
			if ((*node)->isOccupied[T - 1])
			{
				insertNode(node);
				return;
			}
			break;
		case ':':
			var.isKey = false;
			var.isValue = true;
			var.ommitFirstSpace = true;
			break;
		case ';':
			deleteDuplicateAttribute((*node)->blocks[(*node)->exculdedDeletedCounter % T].attribute, var.key, (*node)->blocks[(*node)->exculdedDeletedCounter % T].num_of_attributes);
			(*node)->blocks[(*node)->exculdedDeletedCounter % T].attribute = insertAttribute((*node)->blocks[(*node)->exculdedDeletedCounter % T].attribute, var.key, var.value);
			(*node)->blocks[(*node)->exculdedDeletedCounter % T].num_of_attributes++;
			var.key = "";
			var.value = "";
			var.isKey = true;
			var.isValue = false;
			break;
		case ',':
			if (!var.isValue)
			{
				(*node)->blocks[(*node)->exculdedDeletedCounter % T].selector = insertSelector((*node)->blocks[(*node)->exculdedDeletedCounter % T].selector, var.selector_name);
				(*node)->blocks[(*node)->exculdedDeletedCounter % T].num_of_selectors++;
				var.selector_name = "";
			}
			else
			{
				var.value += line[i];
			}
			break;
		case ' ':
			if ((var.isValue || var.isSelector) && !var.ommitFirstSpace)
			{
				var.value += line[i];
			}
			var.ommitFirstSpace = false;
			break;
		case NUL:
			break;
		case '\n':
			break;
		case '\t':
			break;
		case '\r':
			break;
		default:
			if (var.isSelector)
			{
				var.selector_name += line[i];
			}
			else if (var.isKey)
			{
				var.key += line[i];
			}
			else if (var.isValue)
			{
				var.value += line[i];
			}
			break;
		}
	}
}

// ? - print the number of CSS sections;
void printNumberOfCssSections(int num_of_css_sections)
{
	cout << "? == " << num_of_css_sections << endl;
}

// i,S,? - print the number of selectors for section number i (section and attribute numbers start
// from 1), if there is no such block, skip;
void printNumberOfSelectorsForGivenSectionNumber(int i, Node* node, const int section_number, int num_of_blocks)
{
	if (i - 1 < 0)
	{
		return;
	}
	if (i - 1 >= T)
	{
		Node* temp = node->prev;
		if (temp == NULL)
		{
			return;
		}
		for (int j = 0; j < T; j++)
		{
			if (node->isOccupied[j])
			{
				num_of_blocks++;
			}
		}
		printNumberOfSelectorsForGivenSectionNumber(i - T, temp, section_number, num_of_blocks);
	}
	else
	{
		for (int j = 0; j < T; j++)
		{
			if (node->isOccupied[j])
			{
				num_of_blocks++;
			}
			if (section_number == num_of_blocks)
			{
				i = j + 1;
				break;
			}
		}
		if (node->blocks[i - 1].num_of_selectors != 0)
		{
			cout << section_number << ",S,? == " << node->blocks[i - 1].num_of_selectors << endl;
		}
		return;
	}
}

// i,A,? - print the number of attributes for section number i, if there is no such block or section,
// skip;
void printNumberOfAttributesForGivenSectionNumber(int i, Node* node, const int section_number, int num_of_blocks)
{
	if (i - 1 < 0)
	{
		return;
	}
	if (i - 1 >= T)
	{
		Node* temp = node->prev;
		if (temp == NULL)
		{
			return;
		}
		for (int j = 0; j < T; j++)
		{
			if (node->isOccupied[j])
			{
				num_of_blocks++;
			}
		}
		printNumberOfAttributesForGivenSectionNumber(i - T, temp, section_number, num_of_blocks);
	}
	else
	{
		for (int j = 0; j < T; j++)
		{
			if (node->isOccupied[j])
			{
				num_of_blocks++;
			}
			if (section_number == num_of_blocks)
			{
				i = j + 1;
				break;
			}
		}
		if (node->blocks[i - 1].num_of_attributes != 0)
		{
			cout << section_number << ",A,? == " << node->blocks[i - 1].num_of_attributes << endl;
		}
		return;
	}
	return;
}

// i,S,j - print the j-th selector for the i-th block (section and attribute numbers start from 1), if
// there is no section or selector, skip;
void printSelector(int i, int j, Node* node)
{
	if (i - 1 < 0)
	{
		return;
	}
	// go to the first node
	while (node->next != NULL)
	{
		node = node->next;
	}
	if (i - 1 >= T)
	{
		Node* temp = node->prev;
		if (temp == NULL)
		{
			return;
		}
		printSelector(i - T, j, temp);
	}
	else
	{
		int result = 0;
		if (node->isOccupied[i - 1] == false) return;
		Selector* selector = node->blocks[i - 1].selector;
		// go to first selector
		while (selector->next != NULL)
		{
			selector = selector->next;
		}
		while (selector != NULL)
		{
			result++;
			if (result == j)
			{
				cout << i << ",S," << j << " == " << selector->name << endl;
				break;
			}
			selector = selector->prev;
		}
	}
	return;
}

// i,A,n - print the value of the attribute with the name n for the i-th section, if there is no such
// attribute, skip;
void printAttributeValue(int i, const str& n, Node* node, const int section_number, int num_of_blocks)
{
	if (i - 1 < 0)
	{
		return;
	}
	if (i - 1 >= T)
	{
		if (node->prev == NULL)
		{
			return;
		}
		for (int j = 0; j < T; j++)
		{
			if (node->isOccupied[j])
			{
				num_of_blocks++;
			}
		}
		printAttributeValue(i - T, n, node->prev, section_number, num_of_blocks);
	}
	else
	{
		for (int j = 0; j < T; j++)
		{
			if (node->isOccupied[j])
			{
				num_of_blocks++;
			}
			if (section_number == num_of_blocks)
			{
				i = j + 1;
				break;
			}
		}
		Attribute* head = node->blocks[i - 1].attribute;
		while (head != NULL)
		{
			if (head->name == n)
			{
				cout << section_number << ",A," << n.getData() << " == " << head->value << endl;
				break;
			}
			head = head->next;
		}
		return;
	}
}

// n,A,? - print the total (for all blocks) number of occurrences of attribute named n (duplicates
// should be removed when reading). It can be 0;
void printAttributeOccurrences(const str& n, Node* node)
{
	int result = 0;
	while (node != NULL)
	{
		for (int i = 0; i < T; i++)
		{
			if (node->isOccupied[i])
			{
				Attribute* head = node->blocks[i].attribute;
				while (head != NULL)
				{
					if (head->name == n)
					{
						result++;
						break;
					}
					head = head->next;
				}
			}
		}
		node = node->next;
	}
	cout << n << ",A,? == " << result << endl;
}

// z,S,? - print the total (for all blocks) number of occurrences of selector z. It can be 0;
void printSelectorOccurrences(const str& z, Node* node)
{
	int result = 0;
	while (node != NULL)
	{
		for (int i = 0; i < T; i++)
		{
			if (node->isOccupied[i])
			{
				Selector* selector = node->blocks[i].selector;
				while (selector != NULL)
				{
					if (selector->name == z)
					{
						result++;
					}
					selector = selector->next;
				}
			}
		}
		node = node->next;
	}
	cout << z << ",S,? == " << result << endl;
}

// z,E,n - print the value of the attribute named n for the selector z, in case of multiple
// occurrences of selector z, take the last one. If there is no such attribute, skip;
void printAttributeValueForGivenSelector(const str& z, const str& n, Node* node)
{
	int i = 0;
	str attr_value;
	while (node != NULL)
	{
		for (i = T - 1; i >= 0; i--)
		{
			if (node->isOccupied[i])
			{
				Selector* selector = node->blocks[i].selector;
				while (selector != NULL)
				{
					if (selector->name == z)
					{
						Attribute* attribute = node->blocks[i].attribute;
						while (attribute != NULL)
						{
							if (attribute->name == n)
							{
								cout << z << ",E," << n << " == " << attribute->value << endl;
								return;
							}
							attribute = attribute->next;
						}
					}
					selector = selector->next;
				}
			}
		}
		node = node->next;
	}
	return;
}

// i,D,* - remove the entire section number i (i.e., separators+attributes), after successful
// execution, print "deleted";
void removeEntireSection(int i, Node* node, variables& var, const int section_number, int num_of_blocks)
{
	if (i - 1 < 0)
	{
		return;
	}
	if (i - 1 >= T)
	{
		Node* temp = node->prev;
		if (temp == NULL)
		{
			return;
		}
		for (int j = 0; j < T; j++)
		{
			if (node->isOccupied[j])
			{
				num_of_blocks++;
			}
		}
		removeEntireSection(i - T, temp, var, section_number, num_of_blocks);
	}
	else
	{
		for (int j = 0; j < T; j++)
		{
			if (node->isOccupied[j])
			{
				num_of_blocks++;
			}
			if (section_number == num_of_blocks)
			{
				i = j + 1;
				break;
			}
		}
		deleteBlock(i - 1, node);
		var.num_of_css_sections--;
		cout << section_number << ",D,* == deleted" << endl;
		return;
	}
}

// i,D,n - remove the attribute named n from the i-th section, if the section becomes empty as a
// result of the operation, it should also be removed (along with any selectors), after successful
// execution, print "deleted".
void removeAttribute(int i, const str& n, Node* node, int section_number, int num_of_blocks)
{
	if (i - 1 < 0)
	{
		return;
	}
	if (i - 1 >= T)
	{
		for (int j = 0; j < T; j++)
		{
			if (node->isOccupied[j])
			{
				num_of_blocks++;
			}
		}
		removeAttribute(i - T, n, node->prev, section_number, num_of_blocks);
	}
	else
	{
		for (int j = 0; j < T; j++)
		{
			if (node->isOccupied[j])
			{
				num_of_blocks++;
			}
			if (section_number == num_of_blocks)
			{
				i = j + 1;
				break;
			}
		}
		Attribute*& head = node->blocks[i - 1].attribute;
		Attribute* temp = head;
		while (temp != NULL)
		{
			if (temp->name == n)
			{
				Attribute* del = deleteAttribute(head, temp);
				if (head == del)
				{
					node->blocks[i - 1].attribute = NULL;
				}
				node->blocks[i - 1].num_of_attributes--;
				cout << section_number << ",D," << n << " == deleted" << endl;
				break;
			}
			temp = temp->next;
		}
		return;
	}
}

int stoi(str s)
{
	int multiplier = 1, result = 0;
	for (int i = s.length() - 1; i >= 0; i--)
	{
		result += multiplier * (s[i] - '0');
		multiplier *= 10;
	}
	return result;
}

void IfThereIsZeroAttributesInBlockDeleteIt(int i, Node* node, int& num_of_sections, int num_of_blocks)
{
	if (i - 1 < 0)
	{
		return;
	}
	if (i - 1 >= T)
	{
		for (int j = 0; j < T; j++)
		{
			if (node->isOccupied[j])
			{
				num_of_blocks++;
			}
		}
		IfThereIsZeroAttributesInBlockDeleteIt(i - T, node->prev, num_of_sections, num_of_blocks);
	}
	else
	{
		for (int j = 0; j < T; j++)
		{
			if (node->isOccupied[j])
			{
				num_of_blocks++;
			}
			if (i == num_of_blocks)
			{
				i = j + 1;
				break;
			}
		}
		if (node->blocks[i - 1].num_of_attributes == 0)
		{
			deleteSelectors(i - 1, node);
			node->blocks[i - 1].num_of_selectors = 0;
			deleteAttributes(i - 1, node);
			node->isOccupied[i - 1] = false;
			node->realCounter--;
			num_of_sections--;
			return;
		}
	}
	return;
}

void parseCommandLine(str& line, Node* node, variables& var)
{
	if (line == "****")
	{
		var.isCommand = false;
		return;
	}
	if (line == "?")
	{
		printNumberOfCssSections(var.num_of_css_sections);
		return;
	}
	if (line[0] >= '0' && line[0] <= '9')
	{
		str num = "";
		int i = 0;
		while (line[i] != ',')
		{
			num += line[i];
			i++;
		}
		int n = stoi(num);
		if (line[i] == ',')
		{
			if (line[i + 1] == 'S')
			{
				if (line[i + 2] == ',')
				{
					if (line[i + 3] == '?')
					{
						// go to the first node
						while (node->next != NULL)
						{
							node = node->next;
						}
						printNumberOfSelectorsForGivenSectionNumber(n, node, n, 0);
						return;
					}
					else if (line[i + 3] >= '0' && line[i + 3] <= '9')
					{
						str num2 = "";
						for (unsigned int k = i + 3; k < line.length(); k++)
						{
							num2 += line[k];
						}
						int n2 = stoi(num2);
						// go to the first node
						while (node->next != NULL)
						{
							node = node->next;
						}
						printSelector(n, n2, node);
						return;
					}
				}
			}
			else if (line[i + 1] == 'A')
			{
				if (line[i + 2] == ',')
				{
					if (line[i + 3] == '?')
					{
						// go to the first node
						while (node->next != NULL)
						{
							node = node->next;
						}
						printNumberOfAttributesForGivenSectionNumber(n, node, n, 0);
						return;
					}
					else if ((line[i + 3] >= 'a' && line[i + 3] <= 'z') || (line[i + 3] >= 'A' && line[i + 3] <= 'Z') || line[i + 3] == '#' || line[i + 3] == '.')
					{
						str attr_name = "";
						for (unsigned int j = i + 3; j < line.length(); j++)
						{
							attr_name += line[j];
						}
						// go to the first node
						while (node->next != NULL)
						{
							node = node->next;
						}
						printAttributeValue(n, attr_name, node, n, 0);
						return;
					}
				}
			}
			else if (line[i + 1] == 'D')
			{
				if (line[i + 2] == ',')
				{
					if (line[i + 3] == '*')
					{
						// go to the first node
						while (node->next != NULL)
						{
							node = node->next;
						}
						removeEntireSection(n, node, var, n, 0);
						return;
					}
					else if ((line[i + 3] >= 'a' && line[i + 3] <= 'z') || (line[i + 3] >= 'A' && line[i + 3] <= 'Z') || line[i + 3] == '#' || line[i + 3] == '.')
					{
						str attr_name = "";
						for (unsigned int m = i + 3; m < line.length(); m++)
						{
							attr_name += line[m];
						}
						// go to the first node
						while (node->next != NULL)
						{
							node = node->next;
						}
						removeAttribute(n, attr_name, node, n, 0);
						// go to the first node
						while (node->next != NULL)
						{
							node = node->next;
						}
						IfThereIsZeroAttributesInBlockDeleteIt(n, node, var.num_of_css_sections, 0);
						return;
					}
				}
			}
		}
	}
	else if ((line[0] >= 'a' && line[0] <= 'z') || (line[0] >= 'A' && line[0] <= 'Z') || line[0] == '#' || line[0] == '.')
	{
		str firstWord = "";
		int i = 0;
		while (line[i] != ',')
		{
			firstWord += line[i];
			i++;
		}
		if (line[i + 1] == 'A')
		{
			if (line[i + 2] == ',')
			{
				if (line[i + 3] == '?')
				{
					printAttributeOccurrences(firstWord, node);
					return;
				}
			}
		}
		else if (line[i + 1] == 'S')
		{
			if (line[i + 2] == ',')
			{
				if (line[i + 3] == '?')
				{
					printSelectorOccurrences(firstWord, node);
					return;
				}
			}
		}
		else if (line[i + 1] == 'E')
		{
			if (line[i + 2] == ',')
			{
				if ((line[i + 3] >= 'a' && line[i + 3] <= 'z') || (line[i + 3] >= 'A' && line[i + 3] <= 'Z') || line[i + 3] == '#' || line[i + 3] == '.')
				{
					str sel_name = "";
					for (unsigned int j = i + 3; j < line.length(); j++)
					{
						sel_name += line[j];
					}
					printAttributeValueForGivenSelector(firstWord, sel_name, node);
					return;
				}
			}
		}
		return;
	}
	return;
}

int main()
{
	Node* head = new Node;
	str line;
	struct variables var;
	while (cin.getline(line.data, LINE_SIZE))
	{
		if (!var.isCommand)
		{
			parseCssLine(line, &head, var);
		}
		else
		{
			parseCommandLine(line, head, var);
		}
	}
	deleteAllNodes(head);
	return 0;
}