#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <variant>
#include <algorithm>
#include <bit>

namespace nbt {
	
	enum class SerializationFlag : uint8_t {
		None = 0x0, 
		LittleEndian = 0x1,
		UnnamedRootComponent = 0x2,

		Bedrock = LittleEndian,
		JavaNetwork = UnnamedRootComponent
	};

	using Data = std::vector<uint8_t>;

	class Tag {
	public:
		enum class Type : size_t {
			End = 0, Byte = 1, Short = 2, Int = 3, Long = 4, 
			Float = 5, Double = 6, ByteArray = 7, String = 8,
			List = 9, Compound = 10, IntArray = 11, LongArray = 12
		};

	public:
		static Tag End()                                                   { Tag t;                  t.m_value.emplace<size_t(Type::End)>(0);                      return t; };
		static Tag Byte(int8_t value)                                      { Tag t;                  t.m_value.emplace<size_t(Type::Byte)>(value);                 return t; }
		static Tag Byte(std::string name, int8_t value)                    { Tag t(std::move(name)); t.m_value.emplace<size_t(Type::Byte)>(value);                 return t; }
		static Tag Short(int16_t value)                                    { Tag t;                  t.m_value.emplace<size_t(Type::Short)>(value);                return t; }
		static Tag Short(std::string name, int16_t value)                  { Tag t(std::move(name)); t.m_value.emplace<size_t(Type::Short)>(value);                return t; }
		static Tag Int(int32_t value)                                      { Tag t;                  t.m_value.emplace<size_t(Type::Int)>(value);                  return t; }
		static Tag Int(std::string name, int32_t value)                    { Tag t(std::move(name)); t.m_value.emplace<size_t(Type::Int)>(value);                  return t; }
		static Tag Long(int64_t value)                                     { Tag t;                  t.m_value.emplace<size_t(Type::Long)>(value);                 return t; }
		static Tag Long(std::string name, int64_t value)                   { Tag t(std::move(name)); t.m_value.emplace<size_t(Type::Long)>(value);                 return t; }
		static Tag Float(float value)                                      { Tag t;                  t.m_value.emplace<size_t(Type::Float)>(value);                return t; }
		static Tag Float(std::string name, float value)                    { Tag t(std::move(name)); t.m_value.emplace<size_t(Type::Float)>(value);                return t; }
		static Tag Double(double value)                                    { Tag t;                  t.m_value.emplace<size_t(Type::Double)>(value);               return t; }
		static Tag Double(std::string name, double value)                  { Tag t(std::move(name)); t.m_value.emplace<size_t(Type::Double)>(value);               return t; }
		static Tag ByteArray(std::vector<int8_t> value)                    { Tag t;                  t.m_value.emplace<size_t(Type::ByteArray)>(std::move(value)); return t; }
		static Tag ByteArray(std::string name, std::vector<int8_t> value)  { Tag t(std::move(name)); t.m_value.emplace<size_t(Type::ByteArray)>(std::move(value)); return t; }
		static Tag String(std::string value)                               { Tag t;                  t.m_value.emplace<size_t(Type::String)>(std::move(value));    return t; }
		static Tag String(std::string name, std::string value)             { Tag t(std::move(name)); t.m_value.emplace<size_t(Type::String)>(std::move(value));    return t; }
		static Tag List(std::vector<Tag> value)                            { Tag t;                  t.m_value.emplace<size_t(Type::List)>(std::move(value));      return t; }
		static Tag List(std::string name, std::vector<Tag> value)          { Tag t(std::move(name)); t.m_value.emplace<size_t(Type::List)>(std::move(value));      return t; }
		static Tag Compound(std::vector<Tag> value)                        { Tag t;                  t.m_value.emplace<size_t(Type::Compound)>(std::move(value));  return t; }
		static Tag Compound(std::string name, std::vector<Tag> value)      { Tag t(std::move(name)); t.m_value.emplace<size_t(Type::Compound)>(std::move(value));  return t; }
		static Tag IntArray(std::vector<int32_t> value)                    { Tag t;                  t.m_value.emplace<size_t(Type::IntArray)>(std::move(value));  return t; }
		static Tag IntArray(std::string name, std::vector<int32_t> value)  { Tag t(std::move(name)); t.m_value.emplace<size_t(Type::IntArray)>(std::move(value));  return t; }
		static Tag LongArray(std::vector<int64_t> value)                   { Tag t;                  t.m_value.emplace<size_t(Type::LongArray)>(std::move(value)); return t; }
		static Tag LongArray(std::string name, std::vector<int64_t> value) { Tag t(std::move(name)); t.m_value.emplace<size_t(Type::LongArray)>(std::move(value)); return t; }

		Type type() const noexcept;
		bool isValid() const noexcept;

		int8_t byteValue() const;
		int16_t shortValue() const;
		int32_t intValue() const;
		int64_t longValue() const;
		float floatValue() const;
		double doubleValue() const;
		const std::vector<int8_t>& byteArrayValue() const;
		const std::string& stringValue() const;
		const std::vector<Tag>& listValue() const;
		const std::vector<Tag>& compoundValue() const;
		const std::vector<int32_t>& intArrayValue() const;
		const std::vector<int64_t>& longArrayValue() const;

		const std::string& name() const noexcept;
		bool hasName() const noexcept;
		void setName(const std::string* name);
		void setName(std::string name) noexcept;

		std::string stringify() const;
		Data serialize(SerializationFlag flags = SerializationFlag::None) const;
		static Tag deserialize(const void* data, const void* end, SerializationFlag flags = SerializationFlag::None);

	private:
		Tag() = default;
		Tag(std::string name) : m_hasName(true), m_name(std::move(name)) {}

		void stringify(std::stringstream& stream) const;
		void serialize(std::vector<uint8_t>& data, SerializationFlag flags, bool hideName) const;
		static Tag deserialize(const uint8_t*& data, const void* end, SerializationFlag flags, bool isNameHidden, bool isRoot);
		
		static void writeData(std::vector<uint8_t>& data, const void* src, size_t size);
		static void readData(const uint8_t*& data, const void* end, void* dst, size_t size, bool& error);
		
		template<typename T>
		static void writeNumericalData(std::vector<uint8_t>& data, T src, SerializationFlag flags);
		
		template<typename T>
		static T readNumericalData(const uint8_t*& data, const void* end, SerializationFlag flags, bool& error);

	private:
		std::variant<
			int,
			int8_t,
			int16_t,
			int32_t,
			int64_t,
			float,
			double,
			std::vector<int8_t>,
			std::string,
			std::vector<Tag>,
			std::vector<Tag>,
			std::vector<int32_t>,
			std::vector<int64_t>
		> m_value;

		bool m_hasName = false;
		std::string m_name;

		bool m_error = false;
	};

	inline SerializationFlag operator|(SerializationFlag a, SerializationFlag b) {
		return SerializationFlag(uint8_t(a) | uint8_t(b));
	}

	inline SerializationFlag operator^(SerializationFlag a, SerializationFlag b) {
		return SerializationFlag(uint8_t(a) ^ uint8_t(b));
	}

	inline SerializationFlag operator&(SerializationFlag a, SerializationFlag b) {
		return SerializationFlag(uint8_t(a) & uint8_t(b));
	}

	inline SerializationFlag operator~(SerializationFlag a) {
		return SerializationFlag(~uint8_t(a));
	}

	inline Tag::Type nbt::Tag::type() const noexcept {
		return Type(m_value.index());
	}

	bool Tag::isValid() const noexcept {
		return !m_error;
	}

	inline int8_t Tag::byteValue() const {
		return std::get<size_t(Type::Byte)>(m_value);
	}

	inline int16_t Tag::shortValue() const {
		return std::get<size_t(Type::Short)>(m_value);
	}

	inline int32_t Tag::intValue() const {
		return std::get<size_t(Type::Int)>(m_value);
	}

	inline int64_t Tag::longValue() const {
		return std::get<size_t(Type::Long)>(m_value);
	}

	inline float Tag::floatValue() const {
		return std::get<size_t(Type::Float)>(m_value);
	}

	inline double Tag::doubleValue() const {
		return std::get<size_t(Type::Double)>(m_value);
	}

	inline const std::vector<int8_t>& Tag::byteArrayValue() const {
		return std::get<size_t(Type::ByteArray)>(m_value);
	}

	inline const std::string& Tag::stringValue() const {
		return std::get<size_t(Type::String)>(m_value);
	}

	inline const std::vector<Tag>& Tag::listValue() const {
		return std::get<size_t(Type::List)>(m_value);
	}

	inline const std::vector<Tag>& Tag::compoundValue() const {
		return std::get<size_t(Type::Compound)>(m_value);
	}

	inline const std::vector<int32_t>& Tag::intArrayValue() const {
		return std::get<size_t(Type::IntArray)>(m_value);
	}

	inline const std::vector<int64_t>& Tag::longArrayValue() const {
		return std::get<size_t(Type::LongArray)>(m_value);
	}

	inline const std::string& Tag::name() const noexcept {
		return m_name;
	}

	inline bool Tag::hasName() const noexcept {
		return m_hasName;
	}

	inline void Tag::setName(const std::string* name) {
		if (name == nullptr) {
			m_hasName = false;
			m_name = "";
		} else {
			setName(*name);
		}
	}

	inline void Tag::setName(std::string name) noexcept {
		m_hasName = true;
		m_name = std::move(name);
	}

	inline std::string Tag::stringify() const {
		std::stringstream stream;
		stringify(stream);
		return stream.str();
	}

	inline Data Tag::serialize(SerializationFlag flags) const {
		Data data;
		serialize(data, flags, type() == Type::Compound && bool(flags & SerializationFlag::JavaNetwork));
		return data;
	}

	Tag Tag::deserialize(const void* data, const void* end, SerializationFlag flags) {
		const uint8_t* byteData = static_cast<const uint8_t*>(data);
		return deserialize(byteData, end, flags, false, true);
	}

	inline void Tag::stringify(std::stringstream& stream) const {
		if (hasName())
			stream << name() << ": ";

		switch (type()) {
		case Type::End: stream << "%TAG_END%"; break;
		case Type::Byte: stream << int(byteValue()) << "b"; break;
		case Type::Short: stream << shortValue() << "s"; break;
		case Type::Int: stream << intValue(); break;
		case Type::Long: stream << longValue() << "l"; break;
		case Type::Float: stream << floatValue() << "f"; break;
		case Type::Double: stream << doubleValue(); break;
		case Type::ByteArray:
			{
				stream << "[B;";
				const auto& arr = byteArrayValue();
				for (size_t i = 0; i < arr.size(); ++i)
					stream << int(arr[i]) << ((i == arr.size() - 1) ? "b" : "b, ");
				stream << "]";
			}
			break;

		case Type::String:
			{
				stream << "\"";
				for (auto c : stringValue()) {
					switch (c) {
					case '\'': stream << "\'"; break;
					case '\"': stream << "\\\""; break;
					case '\?"': stream << "\\?"; break;
					case '\\': stream << "\\"; break;
					case '\a': stream << "\\a"; break;
					case '\b': stream << "\\b"; break;
					case '\f': stream << "\\f"; break;
					case '\n': stream << "\\n"; break;
					case '\r': stream << "\\r"; break;
					case '\t': stream << "\\t"; break;
					case '\v': stream << "\\v"; break;
					default: stream << c; break;
					}
				}
				stream << "\"";
			}
			break;

		case Type::List:
			{
				stream << "[";
				const auto& arr = listValue();
				for (size_t i = 0; i < arr.size(); ++i) {
					arr[i].stringify(stream);
					stream << ((i == arr.size() - 1) ? "" : ", ");
				}
				stream << "]";
			}
			break;

		case Type::Compound:
			{
				stream << "{";
				const auto& arr = compoundValue();
				for (size_t i = 0; i < arr.size(); ++i) {
					arr[i].stringify(stream);
					stream << ((i == arr.size() - 1) ? "" : ", ");
				}
				stream << "}";
			}
			break;

		case Type::IntArray:
			{
				stream << "[I;";
				const auto& arr = intArrayValue();
				for (size_t i = 0; i < arr.size(); ++i)
					stream << arr[i] << ((i == arr.size() - 1) ? "" : ", ");
				stream << "]";
			}
			break;

		case Type::LongArray:
			{
				stream << "[L;";
				const auto& arr = longArrayValue();
				for (size_t i = 0; i < arr.size(); ++i)
					stream << arr[i] << ((i == arr.size() - 1) ? "l" : "l, ");
				stream << "]";
			}
			break;
		}
	}

	inline void Tag::serialize(std::vector<uint8_t>& data, SerializationFlag flags, bool hideName) const {
		data.push_back(uint8_t(type()));

		if (!hideName) {
			writeNumericalData(data, uint16_t(m_name.size()), flags);
			writeData(data, m_name.data(), m_name.size());
		}

		switch (type()) {
		case Type::End: data.push_back(0); break;
		case Type::Byte: data.push_back(std::bit_cast<uint8_t>(byteValue())); break;
		case Type::Short: writeNumericalData(data, shortValue(), flags); break;
		case Type::Int: writeNumericalData(data, intValue(), flags); break;
		case Type::Long: writeNumericalData(data, longValue(), flags); break;
		case Type::Float: writeNumericalData(data, floatValue(), flags); break;
		case Type::Double: writeNumericalData(data, doubleValue(), flags); break;
		case Type::ByteArray: 
			{
				const auto& arr = byteArrayValue();
				writeNumericalData(data, int32_t(arr.size()), flags);
				writeData(data, arr.data(), arr.size());
			}
			break;

		case Type::String:
			{
				const auto& str = stringValue();
				writeNumericalData(data, uint16_t(str.size()), flags);
				writeData(data, str.data(), str.size());
			}
			break;

		case Type::List:
			{
				const auto& list = listValue();
				if (list.empty()) {
					uint8_t emptyList[]{ 0,0,0,0,0 };
					writeData(data, emptyList, 5);
				} else {
					data.push_back(uint8_t(list.front().type()));
					writeNumericalData(data, int32_t(list.size()), flags);
					for (const auto& tag : list)
						tag.serialize(data, flags, true);
				}
			}
			break;

		case Type::Compound:
			{
				for(const auto& tag : compoundValue())
					tag.serialize(data, flags, false);
				data.push_back(0);
			}
			break;

		case Type::IntArray:
			{
				const auto& arr = intArrayValue();
				writeNumericalData(data, int32_t(arr.size()), flags);
				for (int32_t i : arr)
					writeNumericalData(data, i, flags);
			}
			break;

		case Type::LongArray:
			{
				const auto& arr = longArrayValue();
				writeNumericalData(data, int32_t(arr.size()), flags);
				for (int64_t i : arr)
					writeNumericalData(data, i, flags);
			}
			break;
		}
	}

	Tag Tag::deserialize(const uint8_t*& it, const void* end, SerializationFlag flags, bool isNameHidden, bool isRoot) {
		if (it + 1 > end) {
			Tag errorTag;
			errorTag.m_error = true;
			return errorTag;
		}
		
		Type type = Type(*it);
		++it;

		if ((type == Type::Compound && bool(flags & SerializationFlag::JavaNetwork) && isRoot) || type == Type::End)
			isNameHidden = true;

		Tag tag;
		if (!isNameHidden) {
			tag.m_hasName = true;
			tag.m_name.resize(readNumericalData<uint16_t>(it, end, flags, tag.m_error));
			readData(it, end, tag.m_name.data(), tag.m_name.size(), tag.m_error);
		}

		if (tag.isValid()) {
			switch (type) {
			case Type::End: tag.m_value.emplace<size_t(Type::End)>(0); break;
			case Type::Byte: if (it + 1 > end) { tag.m_error = true; } else { tag.m_value.emplace<size_t(Type::Byte)>(*it++); } break;
			case Type::Short: tag.m_value.emplace<size_t(Type::Short)>(readNumericalData<int16_t>(it, end, flags, tag.m_error)); break;
			case Type::Int: tag.m_value.emplace<size_t(Type::Int)>(readNumericalData<int32_t>(it, end, flags, tag.m_error)); break;
			case Type::Long: tag.m_value.emplace<size_t(Type::Long)>(readNumericalData<int64_t>(it, end, flags, tag.m_error)); break;
			case Type::Float: tag.m_value.emplace<size_t(Type::Float)>(readNumericalData<float>(it, end, flags, tag.m_error)); break;
			case Type::Double: tag.m_value.emplace<size_t(Type::Double)>(readNumericalData<double>(it, end, flags, tag.m_error)); break;
			case Type::ByteArray:
				{
					std::vector<int8_t> byteArray;
					byteArray.resize(std::max(readNumericalData<int32_t>(it, end, flags, tag.m_error), 0));
					readData(it, end, byteArray.data(), byteArray.size(), tag.m_error);
					tag.m_value.emplace<size_t(Type::ByteArray)>(std::move(byteArray));
				}
				break;

			case Type::String:
				{
					std::string str;
					str.resize(readNumericalData<uint16_t>(it, end, flags, tag.m_error));
					readData(it, end, str.data(), str.size(), tag.m_error);
					tag.m_value.emplace<size_t(Type::String)>(std::move(str));
				}
				break;

			case Type::List:
				{
					Type listType = Type(*it++);
					if (it >= end)
						tag.m_error = true;

					std::vector<Tag> tags;
					int32_t size = std::max(readNumericalData<int32_t>(it, end, flags, tag.m_error), 0);
					tags.reserve(size);

					for (size_t i = 0; i < size && tag.isValid(); ++i) {
						Tag child = deserialize(it, end, flags, true, false);
						if (!child.isValid() || child.type() != listType)
							tag.m_error = true;
						tags.emplace_back(std::move(child));
					}

					tag.m_value.emplace<size_t(Type::List)>(std::move(tags));
				}
				break;

			case Type::Compound:
				{
					std::vector<Tag> tags;

					while (tag.isValid()) {
						Tag child = deserialize(it, end, flags, false, false);
						if (!child.isValid())
							tag.m_error = true;
						if (child.type() == Type::End)
							break;
						tags.emplace_back(std::move(child));
					}

					tag.m_value.emplace<size_t(Type::Compound)>(std::move(tags));
				}
				break;

			case Type::IntArray:
				{
					std::vector<int32_t> arr;
					arr.resize(std::max(readNumericalData<int32_t>(it, end, flags, tag.m_error), 0));

					for (size_t i = 0; i < arr.size() && tag.isValid(); ++i)
						arr[i] = readNumericalData<int32_t>(it, end, flags, tag.m_error);

					tag.m_value.emplace<size_t(Type::IntArray)>(std::move(arr));
				}
				break;

			case Type::LongArray:
				{
					std::vector<int64_t> arr;
					arr.resize(std::max(readNumericalData<int32_t>(it, end, flags, tag.m_error), 0));

					for (size_t i = 0; i < arr.size() && tag.isValid(); ++i)
						arr[i] = readNumericalData<int64_t>(it, end, flags, tag.m_error);

					tag.m_value.emplace<size_t(Type::LongArray)>(std::move(arr));
				}
				break;

			default:
				break;
			}

		}
		return tag;
	}

	inline void Tag::writeData(std::vector<uint8_t>& data, const void* src, size_t size) {
		size_t offset = data.size();
		data.resize(data.size() + size);
		memcpy(data.data() + offset, src, size);
	}

	inline void Tag::readData(const uint8_t*& it, const void* end, void* dst, size_t size, bool& error) {
		if (it + size > end || error) {
			error = true;
			return;
		}
		memcpy(dst, it, size);
		it += size;
	}

	template<typename T>
	inline void Tag::writeNumericalData(std::vector<uint8_t>& data, T src, SerializationFlag flags) {
		writeData(data, &src, sizeof(T));

		if constexpr (std::endian::native == std::endian::little) {
			if (!bool(flags & SerializationFlag::Bedrock)) {
				std::reverse(data.begin() + data.size() - sizeof(T), data.begin() + data.size());
			}
		} else {
			if (bool(flags & SerializationFlag::Bedrock)) {
				std::reverse(data.begin() + data.size() - sizeof(T), data.begin() + data.size());
			}
		}
	}

	template<typename T>
	inline T Tag::readNumericalData(const uint8_t*& it, const void* end, SerializationFlag flags, bool& error) {
		union {
			T t;
			uint8_t[sizeof(T)] bytes;
		} data;

		readData(it, end, &data.t, sizeof(T), error);

		if (!error) {
			if constexpr (std::endian::native == std::endian::little) {
				if (!bool(flags & SerializationFlag::Bedrock)) {
					std::reverse(data.bytes, data.bytes + sizeof(T));
				}
			} else {
				if (bool(flags & SerializationFlag::Bedrock)) {
					std::reverse(data.bytes, data.bytes + sizeof(T));
				}
			}
			return data.t;
		} else {
			return T(0);
		}
	}
}