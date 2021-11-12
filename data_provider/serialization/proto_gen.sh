#!/bin/sh
PROTOBUF=${HOME}/libs/protobuf/protobuf-3.18.0
PROTOC=${PROTOBUF}/bin/protoc

SRC_DIR=`pwd`
PROTO_DIR=${SRC_DIR}/proto
DST_DIR=${SRC_DIR}/proto_gen
IMPORT_PATH=${PROTO_DIR}

cd ${DST_DIR}
rm -f *.pb.h *.pb.cc
cd ${SRC_DIR}
$PROTOC --proto_path=${IMPORT_PATH} -I=$SRC_DIR --cpp_out=$DST_DIR $PROTO_DIR/*.proto


