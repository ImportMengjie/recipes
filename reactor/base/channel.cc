#include "channel.h"

#include "eventLoop.h"

using namespace mengjie::reactor;

void mengjie::reactor::Channel::update() { _loop->updateChannel(this); }
