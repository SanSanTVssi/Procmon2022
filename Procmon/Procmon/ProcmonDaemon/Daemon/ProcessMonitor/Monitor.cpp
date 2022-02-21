///
///  Monitor.cpp
///  DaemonESTest
///
///  Copyright © 2022 Aai2002. All rights reserved.
///  25.01.2022.
///

#include "Monitor.hpp"

int Monitor::init(decltype(_handle_event) handle_event,
          unsigned int size,
          es_event_type_t* Events)
{
    if(!_is_inited) {
        _handle_event = handle_event;
        _size = size;
        _events = Events;
        _CreateNewESClient();
        
        if (_events == nullptr) {
            _SetDefaultEvents();
        }
        
        _is_inited = true;
    }
    return _is_inited;
}

Monitor::~Monitor() {
    es_delete_client(_client);
    delete[] _events;
}

int Monitor::operator()() {
    if(!_is_subscribed) {
        if (es_subscribe (
                _client,
                _events,
                _size) != ES_RETURN_SUCCESS
            )
        {
            es_delete_client(_client);
            throw std::runtime_error("Failed to subscribe to events");
        }
        _is_subscribed = true;
    }
    return _is_subscribed;
}

void Monitor::operator()(std::function<void()> dispatch_function) {
    operator()();
    dispatch_function();
}

void Monitor::_CreateNewESClient() {
    auto result =
    es_new_client(
                  &_client,
                  ^(es_client_t *c,
                    const es_message_t *msg) {
        _handle_event(c, msg);
    });
    
    if (result != ES_NEW_CLIENT_RESULT_SUCCESS) {
        throw std::runtime_error("Failed to create new ES client");
    }
}

void Monitor::_SetDefaultEvents() {
    _events = new es_event_type_t[_size] {
        ES_EVENT_TYPE_NOTIFY_EXEC,
        ES_EVENT_TYPE_NOTIFY_EXIT
    };
}
