# Plat4m Topic Module

The **Plat4m::Topic** module represents a Topic in a publish-subscribe messaging system. Plat4m Topics are very lightweight and simply consist of an application-unique unsigned 32-bit ID and a linked list of subscriber callbacks. Data published to a Topic is called a **sample**.

For simplicity, subscriber callbacks are executed from the context of the caller of the `Topic::publish()` method. More complex behavior can be layered onto Topics by injecting code on the receiving end of a Topic sample. See the **TopicSubscriber** and **TopicSubscriberThread** sections below for more information.

To create a Topic, the Topic::create() method must be called and a reference to the created Topic is returned. This allows Topics to be subscribed to before they have been instantiated by the owner.

## Creating a Topic

Here's an example of creating a Topic with a data type of `std::uint8_t`:

``` C++
TopicBase::Id topicId = 42;
Topic<std::uint8_t>& topic = Topic<std::uint8_t>::create(topicId);
```

## Publishing to a Topic

To publish a sample to the example Topic:

``` C++
const std::uint8_t sample = 8;
topic.publish(sample);
```

Unlike many other pub-sub frameworks, Topic publishing isn't done anonymously based on the topic ID. Instead, the owner of the Topic calls the `publish()` directly, thereby removing the need for a search of the Topic list.

## Subscribing to a Topic

To subscribe to a Topic, you need a callback function or method with the following prototype:

``` C++
void sampleCallback(const Plat4m::TopicSample<std::uint8_t>& sample)
{
    ...
}
```

Then call `Topic::subscribe()` using the `Plat4m::createCallback()` helper:

``` C++
Topic<std::uint8_t>::subscribe(createCallback(&sampleCallback));
```

Now, every time `topic.publish()` is called, `sampleCallback()` will be called with the latest sample.

**Note:** Subscriber callbacks are called in a linear fashion based on the order of subscription, which introduces a de facto priority to the subscribers. Future versions of the Topic class will allow reordering of the subscribers for priority.

## TopicSubscriber

The **Plat4m::TopicSubscriber** module encapsulates a subscriber callback and adds helpful features:

* Enabling/disabling via subclass of **Plat4m::Module**
* Downsamling factor
  * Ex. If downsample factor = 2, the subscriber callback will be called for every other sample

## TopicSubscriberThread

**Plat4m::TopicSubscriberThread** is a subclass of **Plat4m::TopicSubscriber** that adds a threaded callback implementation. This is accomplished by creating a **Plat4m::Thread** that waits until a sample has been pushed into a **Plat4m::Queue**. When the sample callback for **TopicSubscriberThread** is called, it pushes the sample into the queue, which wakes the thread and calls the subscriber callback in the thread context.

**Note:** Due to the topic publishing occurring in the caller's context and the de facto priority of the subscribers described above, it's possible to have a **TopicSubscriberThread** with a higher thread priority than the publishing thread that might not be executed right away, despite it having a higher priority. For example, if a non-threaded subscriber subscribed to a particular topic before a threaded one, the non-threaded subscriber would be called first in the topic publisher context, then the threaded subscriber would be called second at which point the thread context would switch.

## Examples

* [TopicTest](../Test/Acceptance_Tests/TopicTest.cpp)
* [TopicSubscriberTest](../Test/Acceptance_Tests/TopicSubscriberTest.cpp)
* [TopicSubscriberThreadTest](../Test/Acceptance_Tests/TopicSubscriberThreadTest.cpp)
