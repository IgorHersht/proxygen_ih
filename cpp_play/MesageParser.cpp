
#include <string>
#include <iostream>
#include <map>
#include <boost/tokenizer.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
// header files
struct MsConsts{
	
	enum MsType {
		UnknownMsType = -1,
		Logon,
		NewOrderSingle,
		// ...
		MsTypeNum
	};
	
	enum FieldType {
		UnknownFieldType = -1,
		Char,
		Int,
		String,
		UTCTimestampt,
		// ...
		FieldTypeNum
	};
	
	enum Tag {
		UnknownTag = -1,
		BeginString = 8,
		BodyLength = 9,
		MsgSeqNum = 34,
		MsgType = 35,
		SenderCompID =49,
		SenderSubID = 50,
		SendingTime = 52,
		TargetCompID =56,
		TargetSubID = 57,
		SenderLocationID =142,
		// ...
		TagNum
	};

	static FieldType getFieldType(Tag tag)
	{
		switch( tag)
		{
			case BeginString:			return String;
			case BodyLength:			return Int;
			case MsgSeqNum:				return Int;
			case MsgType:				return String;
			case SenderCompID:			return String;
			case SenderSubID:			return String;
			case SendingTime:			return UTCTimestampt;
			case TargetCompID:			return String;
			case TargetSubID:			return String;
			case SenderLocationID:		return String;
			//...		                                                             
		}
		return  UnknownFieldType;
	}

	static MsType getMsType(const std::string &type) // Assuming a type is just one char. (True for FIX 4.2)
	{
		if(type.length() == 0) return UnknownMsType;
		switch( type.at(0))
		{
			case 'A':	return Logon;
			// ..                                
		}
		return  UnknownMsType;
	}

	const static std::string EXCANGE;
	const static std::string FIX_VERSION;
	const static std::string SEPARATOR;
	const static std::string INVALID_VALUE;
};


class MsBase{
public:
	typedef boost::shared_ptr<MsBase> MsPtr;
	typedef MsConsts::MsType	MsType;
	typedef MsConsts::FieldType FieldType;
	typedef MsConsts::Tag		Tag;
	typedef std::map<Tag,std::string> TagMap;
    static MsPtr makeMessage(const std::string &ms); 
	MsBase();
	virtual const std::string& getFieldValue(Tag tag) const;
protected:
	void parse(const std::string &ms);	
	void setField(const std::string &fld);
    void setValue(MsConsts::Tag tag,  const std::string &v);
	TagMap		m_tagMap;
private:
	static MsType getMsType(const std::string &ms); 
};

template <MsBase::MsType> class Message;

template<> class Message<MsConsts::Logon> : public MsBase{
		typedef MsBase Super;
		typedef boost::shared_ptr< Message<MsConsts::Logon> > MsPtr;
public:
		enum { TYPE = MsConsts::Logon };
		Message();
};


// cpp files
using namespace std;
using namespace boost;
const string MsConsts::SEPARATOR("^");
const string MsConsts::INVALID_VALUE;
const string MsConsts::EXCANGE("CBOE");
const string MsConsts::FIX_VERSION("4.2");


 MsConsts::MsType MsBase::getMsType(const string &ms)
 {
	 const string bStr = MsConsts::SEPARATOR + lexical_cast<string>(int(MsConsts::MsgType)) + "=";
	 string::size_type bgn = ms.find(bStr);
	 if(bgn ==  string::npos) return MsConsts::UnknownMsType;
	 const string::size_type end = ms.find(MsConsts::SEPARATOR, bgn +1);
	 if(end ==  string::npos) return MsConsts::UnknownMsType;
	 
	 bgn = bgn + bStr.length();
	 if((end - bgn) < 1 ) return MsConsts::UnknownMsType;
	 
	 const string type = ms.substr(bgn,end - bgn);
	 return MsConsts::getMsType(type);
	 
 }

 MsBase::MsBase()// should use initialization with a static map
 {
	 m_tagMap.insert(TagMap::value_type(MsConsts::BeginString, MsConsts::INVALID_VALUE));
	 m_tagMap.insert(TagMap::value_type(MsConsts::BodyLength, MsConsts::INVALID_VALUE));
	 m_tagMap.insert(TagMap::value_type(MsConsts::MsgSeqNum, MsConsts::INVALID_VALUE));
	 m_tagMap.insert(TagMap::value_type(MsConsts::MsgType, MsConsts::INVALID_VALUE));
	 m_tagMap.insert(TagMap::value_type(MsConsts::SenderCompID, MsConsts::INVALID_VALUE));
	 m_tagMap.insert(TagMap::value_type(MsConsts::TargetCompID, MsConsts::INVALID_VALUE));
	 
 }
MsBase::MsPtr MsBase::makeMessage(const string &ms)
{
	const MsConsts::MsType type = getMsType(ms);
	if(type == MsConsts::UnknownMsType){
		throw "Unknown Message Type "; // should be specific exception
	}
	MsBase::MsPtr ptr; 
	switch( type)
	{		
		case MsConsts::Logon:{
			ptr = Message<MsConsts::Logon>::MsPtr(new Message<MsConsts::Logon> );
			break;
	   }
	//...
	 default: throw "Cannot create message object  "; // should be specific exception	                                                             
	}
	ptr->parse(ms);
	return ptr;
}

 const string& MsBase::getFieldValue(MsBase::Tag tag) const
  {
	TagMap::const_iterator pos = m_tagMap.find(tag);
	if(pos == m_tagMap.end()) return MsConsts::INVALID_VALUE;; // should be specific exception
	return pos->second ;
  }
 
 void MsBase::parse(const std::string &ms) 
 {
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep(MsConsts::SEPARATOR.c_str());
	tokenizer tokens(ms, sep);
	for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter){
		setField(*tok_iter);
	}
 }

 void MsBase::setField(const std::string &fld)
{
	 const string::size_type len = fld.length();
	 const string::size_type pos = fld.find('=');
	 if((pos ==  string::npos) || (pos < 1) || ((pos +2) > len ) ){
		 throw ("Invalid field " + fld); // should be specific exception
	 }
	 const int tag = lexical_cast<int>(fld.substr(0,pos));
	 const string value = fld.substr(pos +1); 
	 setValue(MsConsts::Tag(tag), value);

}
  void MsBase::setValue(MsConsts::Tag tag,  const std::string &value)
{	
	TagMap::iterator pos = m_tagMap.find(tag);
	if(pos == m_tagMap.end()) throw ("Invalid tag " + tag); // should be specific exception
	pos->second = value;
	
}

  Message<MsConsts::Logon>::Message()// should use initialization with a static map
 {
	 m_tagMap.insert(TagMap::value_type(MsConsts::SenderSubID, MsConsts::INVALID_VALUE));
	 m_tagMap.insert(TagMap::value_type(MsConsts::TargetSubID, MsConsts::INVALID_VALUE));
	 m_tagMap.insert(TagMap::value_type(MsConsts::SendingTime, MsConsts::INVALID_VALUE));	 
 }


  

int main() 
{
	MsBase::MsPtr msg =MsBase::makeMessage("8=FIX.4.2^9=102^35=A^49=SMJN01^56=PFIX01^34=27^50=smg:son123^57=PROD^52=20100401-12:01:01:001");
	cout << msg->getFieldValue(MsConsts::SenderCompID)<<endl;
	cout << msg->getFieldValue(MsConsts::SenderSubID)<<endl;
	

	return 0;
}
