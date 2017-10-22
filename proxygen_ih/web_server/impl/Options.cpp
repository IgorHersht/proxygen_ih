/*
MIT License

Copyright @ 2017 Igor Hersht

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <shared/Options.h>

DEFINE_string(HTTP_SERVER_APPLICATION_NAME, "",
		"Application absolute paht name ");
DEFINE_string(HTTP_SERVER_IP, "", "IP/Hostname to bind to");
DEFINE_int32(HTTP_SERVER_PORT, 0, "Port to listen on with HTTP protocol");


DEFINE_int64(HTTP_SERVER_IDLE_TIMEOUT, 1000,
		"How long (in ms) to keep idle connections around before throwing them away");
DEFINE_int32(HTTP_SERVER_LISTEN_BACKLOG, 8192,
		" TCP server socket backlog to start with");
DEFINE_int64(HTTP_SERVER_CHILD_TIMEOUT, 300000,
		"How long (in ms) to keep keep child process before killing it");
DEFINE_string(HTTP_SERVER_WD, "", "Change the application working directory.");
DEFINE_bool(HTTP_SERVER_SUPPORTS_CONNECT, false,
		"Set to true if you want to support CONNECT request");
// threads
DEFINE_int32(HTTP_SERVER_IO_THREADS, 0,
		"Number of threads to listen on. Numbers <= 0 will use the number of cores.");
DEFINE_bool(HTTP_SERVER_ASYNC_RESPONSE, true,
		"Set to true if you want to use async response");
DEFINE_int32(HTTP_SERVER_CPU_THREADS_PER_IO_THREAD, 64,
		"Number of cpu threads per IO_THREAD. ");
// request timeout
DEFINE_int64(HTTP_SERVER_TIMEOUT_REQUST, 0,
		"Default request timeout. 0 means no timeout ");
DEFINE_uint64(HTTP_SERVER_TIMEOUT_REQUST_CODE, 503,
		"Status code on request timeout. ");
// counters
DEFINE_bool(HTTP_SERVER_ENABLE_CONNECT_COUNTER, false,
		"Set to true if you want to enable");
DEFINE_bool(HTTP_SERVER_ENABLE_REGECTED_CONNECT_COUNTER, false,
		"Set to true if you want to enable");
DEFINE_bool(HTTP_SERVER_ENABLE_REQUEST_COUNTER, false,
		"Set to true if you want to enable");
DEFINE_bool(HTTP_SERVER_ENABLE_TIMEOUTED_REQUEST_COUNTER, false,
		"Set to true if you want to enable");
// limits
DEFINE_uint64(HTTP_SERVER_MAX_CONNECTIONS_PER_IO_THREAD, 0,
		"0 means no limit  ");
DEFINE_uint64(HTTP_SERVER_MAX_REQUESTS_PER_IO_THREAD, 0, ". 0 means no limit ");
DEFINE_uint64(HTTP_SERVER_MAX_TIMEOUTED_REQUEST_RATIO, 0, "0 means no limit ");

namespace shared {

}
