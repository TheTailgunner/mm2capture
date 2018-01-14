#ifndef FEEDFACTORY_H
#define FEEDFACTORY_H

namespace MM2Capture {

class FeedFactory {
public:
  FeedFactory() = delete;
  FeedFactory(const FeedFactory &) = delete;
  FeedFactory &operator==(const FeedFactory &) = delete;
  ~FeedFactory() = delete;
};
}
#endif // FEEDFACTORY_H
