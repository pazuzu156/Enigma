#ifndef MESSAGESTATUS_HPP_INCLUDED
#define MESSAGESTATUS_HPP_INCLUDED

enum DllExport MessageStatus
{
    STATUS_OK=0,
    STATUS_UNKNOWN_FAIL=1,
    STATUS_SECURITY_FAIL=2,
    STATUS_DATABASE_FAIL=3,
    STATUS_NETWORK_FAIL=4,
    STATUS_VERSION_FAIL=5
};

#endif // MESSAGESTATUS_HPP_INCLUDED
