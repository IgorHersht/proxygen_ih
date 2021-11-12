#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include "message.pb.h"

namespace gprbf = google::protobuf;

namespace data_provider {
namespace protobuf {
/////////////////////////////////////////////////////////////////////////
//DB serializer.Get all data, serialise the data.
std::string getAlldDataSerzd() {
    RawCollatItem item;// should be local to be cleaned and deleted

    // must not delete - message owns the values
    // scalar
    OptI *optIPtr1 = new OptI;
    optIPtr1->set_value(1);
    item.set_allocated_i1(optIPtr1);
    OptI *optIPtr2 = new OptI;
    item.set_allocated_i2(optIPtr2);// optIPtr2->set_value(2); i2 = null
    OptF *optFPtr1 = new OptF;
    optFPtr1->set_value(1);
    item.set_allocated_f1(optFPtr1);
    OptF *optFPtr2 = new OptF;
    optFPtr2->set_value(2);
    item.set_allocated_f2(optFPtr2);
    OptS *optSPtr1 = new OptS;
    optSPtr1->set_value("SSSSSSSS1");
    item.set_allocated_s1(optSPtr1);
    OptS *optSPtr2 = new OptS;
    optSPtr2->set_value("SSSSSSSS2");
    item.set_allocated_s2(optSPtr2);
    //vectors
    RptI *rptIPtr = new RptI;
    OptI *optIPtr3 = rptIPtr->add_value();
    optIPtr3->set_value(1);
    OptI *optIPtr4 = rptIPtr->add_value(); //optIPtr4->set_value(2); // null value at index=1;
    OptI *optIPtr5 = rptIPtr->add_value();
    optIPtr5->set_value(3);
    item.set_allocated_vi1(rptIPtr);

    RptF *rptFPtr = new RptF;
    OptF *optFPtr3 = rptFPtr->add_value();
    optFPtr3->set_value(1);
    OptF *optFPtr4 = rptFPtr->add_value(); //optFPtr4->set_value(2); // null value at index=1;
    OptF *optFPtr5 = rptFPtr->add_value();
    optFPtr5->set_value(3);
    item.set_allocated_vf1(rptFPtr);

    RptS *rptSPtr = new RptS;
    OptS *optSPtr3 = rptSPtr->add_value();
    optSPtr3->set_value("SSSSSSSS11");
    OptS *optSPtr4 = rptSPtr->add_value(); //optFPtr4->set_value(2); // null value at index=1;
    OptS *optSPtr5 = rptSPtr->add_value();
    optSPtr5->set_value("SSSSSSSS13");
    item.set_allocated_vs1(rptSPtr);

    std::string data;
    item.SerializeToString(&data);
    return data;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Data Provider. De-serialize all data, get only requered data (FieldsInfos),  serialize requered data.

struct FieldInfo {
    typedef gprbf::FieldDescriptor::Type Type;
    typedef gprbf::FieldDescriptor::Label Label;

    FieldInfo(Type type, Label label = gprbf::FieldDescriptor::LABEL_OPTIONAL, bool mandatory = false) :
            _type(type), _label(label), _mandatory(mandatory) {

    }

    const Type _type;
    const Label _label;
    const bool _mandatory{false};
};

typedef std::map<std::string, FieldInfo> FieldsInfos;

////

RawCollatItem getAllFields(const std::string &data) {
    RawCollatItem item;
    item.ParseFromString(data);
    std::cout << "allFields serialized size =" << data.size() << std::endl;
    std::cout << "allFields  =" << item.DebugString() << std::endl;
    return item;
}

// validate field with respect to requered. Defined in proto
bool validateField(const gprbf::FieldDescriptor *fieldDescriptor, const std::string &fieldId,
                   const FieldInfo &requeredFieldInfo) {
    if (!fieldDescriptor) { // validate id
        assert(!requeredFieldInfo._mandatory);
        return false;
    }
    // const FieldInfo::Type requeredFieldType = requeredItem.second._type;
    // const FieldInfo::Label requeredFieldLabel = requeredItem.second._label;
//    const FieldInfo::Label receivedFieldLabel = fieldDescriptor->label();
//    assert(requeredFieldLabel == receivedFieldLabel);
//    // validate type. defined in proto
//    //const OneofDescriptor* containing_oneof() const;
//    const FieldInfo::Type receivedFieldType = fieldDescriptor->type();
//    if( receivedFieldType == gprbf::FieldDescriptor::TYPE_MESSAGE){
//
//    }
//
//    //assert(requeredFieldType == receivedFieldType);
    return true;
}

std::string getSomeData(const std::string &dataIn, const FieldsInfos &requeredFieldInfos) {
    RawCollatItem item = getAllFields(dataIn);
    const gprbf::Descriptor *itemDescriptor = item.GetDescriptor();
    assert(itemDescriptor != nullptr);

    // collect and validate field descriptors (with respect to requered)
    std::vector<const gprbf::FieldDescriptor *> requeredFieldDescriptors;
    for (const auto &element: requeredFieldInfos) {
        const std::string &requeredFieldId = element.first;
        const gprbf::FieldDescriptor *fieldDescriptor = itemDescriptor->FindFieldByName(requeredFieldId);
        // validate
        const bool requered = validateField(fieldDescriptor, requeredFieldId, element.second);
        if (requered) {
            requeredFieldDescriptors.push_back(fieldDescriptor);
        }
    }
    // set values in not ignored fields. item became invalid (SwapFields). If needed to re-use use a copy of item
    RawCollatItem requeredItem;
    const gprbf::Reflection *reflection = item.GetReflection();
    reflection->SwapFields(&item, &requeredItem, requeredFieldDescriptors);
    // serialize
    std::string dataOut;
    requeredItem.SerializeToString(&dataOut);
    std::cout << "requeredItem serialized size =" << dataOut.size() << std::endl;
    std::cout << "requeredItem  =" << requeredItem.DebugString() << std::endl;
    return dataOut;
}

}//namespace data_provider
}//namespace protobuf
/////////////////////////// test
namespace prbf = data_provider::protobuf;
void test1()
{
    try{
    /// DB serializer. All data
    std::string allDataSerzd = prbf::getAlldDataSerzd();
    int szAll = allDataSerzd.size();
    // set filter
    prbf::FieldsInfos requeredFieldsInfo;
    requeredFieldsInfo.emplace("i1", prbf::FieldInfo(gprbf::FieldDescriptor::TYPE_INT32) );// 4
    requeredFieldsInfo.emplace("vi1", prbf::FieldInfo(gprbf::FieldDescriptor::TYPE_INT32, gprbf::FieldDescriptor::LABEL_REPEATED ) );// + 12 = 16
    // Data Provider: someData
    getSomeData(allDataSerzd, requeredFieldsInfo);
    int i =1;
    }catch(...){
        int i =1;
    }

}

int main(int argc, char* argv[]) {

    GOOGLE_PROTOBUF_VERIFY_VERSION;//
    test1();
    int i =0;

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
/*
 * allItems serialized size =108
allItems  =i1 {
  value: 1
}
i2 {
}
f1 {
  value: 1
}
f2 {
  value: 2
}
s1 {
  value: "SSSSSSSS1"
}
s2 {
  value: "SSSSSSSS2"
}
vi1 {
  value {
    value: 1
  }
  value {
  }
  value {
    value: 3
  }
}
vf1 {
  value {
    value: 1
  }
  value {
  }
  value {
    value: 3
  }
}
vs1 {
  value {
    value: "SSSSSSSS11"
  }
  value {
  }
  value {
    value: "SSSSSSSS13"
  }
}

someItems serialized size =16
someItems  =i1 {
  value: 1
}
vi1 {
  value {
    value: 1
  }
  value {
  }
  value {
    value: 3
  }
}

 */