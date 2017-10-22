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

#pragma once

#include <vector>
#include <map>
#include <string>

#ifndef PROTOCOL_CONTINUE

/* HTTP status codes */
#define PROTOCOL_CONTINUE 100
#define PROTOCOL_SWITCHING 101
#define PROTOCOL_OK 200
#define PROTOCOL_CREATED 201
#define PROTOCOL_ACCEPTED 202
#define PROTOCOL_NONAUTHORITATIVE 203
#define PROTOCOL_NO_RESPONSE 204
#define PROTOCOL_NO_CONTENT 204
#define PROTOCOL_RESET_CONTENT 205
#define PROTOCOL_PARTIAL_CONTENT 206
#define PROTOCOL_MULTI_STATUS 207
#define PROTOCOL_MULTIPLE_CHOICES 300
#define PROTOCOL_MOVED_PERMANENTLY 301
#define PROTOCOL_REDIRECT 302
#define PROTOCOL_SEE_OTHER 303
#define PROTOCOL_NOT_MODIFIED 304
#define PROTOCOL_USE_PROXY 305
#define PROTOCOL_TEMPORARY_REDIRECT 307
#define PROTOCOL_BAD_REQUEST 400
#define PROTOCOL_UNAUTHORIZED 401
#define PROTOCOL_PAYMENT_REQUIRED 402
#define PROTOCOL_FORBIDDEN 403
#define PROTOCOL_NOT_FOUND 404
#define PROTOCOL_METHOD_NOT_ALLOWED 405
#define PROTOCOL_NOT_ACCEPTABLE 406
#define PROTOCOL_PROXY_UNAUTHORIZED 407
#define PROTOCOL_REQUEST_TIMEOUT 408
#define PROTOCOL_CONFLICT 409
#define PROTOCOL_GONE 410
#define PROTOCOL_LENGTH_REQUIRED 411
#define PROTOCOL_PRECONDITION_FAIL 412
#define PROTOCOL_ENTITY_TOO_LARGE 413
#define PROTOCOL_URI_TOO_LARGE 414
#define PROTOCOL_UNSUPPORTED_MEDIA_TYPE 415
#define PROTOCOL_REQUESTED_RANGE_NOT_SATISFIABLE 416
#define PROTOCOL_EXPECTATION_FAILED 417
#define PROTOCOL_LOCKED 423
#define PROTOCOL_FAILED_DEPENDENCY 424
#define PROTOCOL_TOO_MANY_REQUESTS 429
#define PROTOCOL_SERVER_ERROR 500
#define PROTOCOL_NOT_IMPLEMENTED 501
#define PROTOCOL_BAD_GATEWAY 502
#define PROTOCOL_SERVICE_UNAVAILABLE 503
#define PROTOCOL_GATEWAY_TIMEOUT 504
#define PROTOCOL_VERSION_NOT_SUPPORTED 505
#define PROTOCOL_INSUFFICIENT_STORAGE 507
#define PROTOCOL_TIMEOUT 524
#endif

#ifndef REQ_PROCEED
/* The function performed its task, proceed with the request */
#define REQ_PROCEED 0
/* The entire request should be aborted: An error occurred */
#define REQ_ABORTED -1
/* The function performed no task, but proceed anyway */
#define REQ_NOACTION -2
/* Tear down the session and exit */
#define REQ_EXIT -3
/* Restart the entire request-response process */
#define REQ_RESTART -4
/* Too busy to execute this now */
#define REQ_TOOBUSY -5
#endif

const std::string EmptyStr;

