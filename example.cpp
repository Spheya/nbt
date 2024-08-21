#include <iostream>

#include <nbt.hpp>

int main() {
	nbt::Tag test1 = nbt::Tag::Compound("hello world", {
		nbt::Tag::ByteArray("numbers", {1,2,3,4,5}),
		nbt::Tag::List("list", {
			nbt::Tag::Compound({
				nbt::Tag::Float("float", 4.0f),
				nbt::Tag::Int("int", 4),
			}),
			nbt::Tag::Compound({
				nbt::Tag::Float("float", 4.12345345f),
				nbt::Tag::Int("int", 5),
			})
		}),
		nbt::Tag::String("name", "Spheya"),
		nbt::Tag::String("line", "this is a string\n\r"),
		nbt::Tag::IntArray("cool values", { 421, 534 }),
		nbt::Tag::Double("pi", 3.14159265359)
	});

	nbt::Data data = test1.serialize();
	nbt::Tag test2 = nbt::Tag::deserialize(data.data(), data.data() + data.size());

	std::cout << test1.stringify() << std::endl;
	std::cout << test2.stringify() << std::endl;
}