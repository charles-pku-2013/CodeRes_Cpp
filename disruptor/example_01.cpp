/*
 * c++ -o /tmp/test /tmp/test.cpp -lDisruptor -lboost_system -std=c++11 -O3 -g
 */
#include <iostream>
#include <memory>

#include <Disruptor/AggregateEventHandler.h>
#include <Disruptor/AlertException.h>
#include <Disruptor/ArgumentException.h>
#include <Disruptor/ArgumentNullException.h>
#include <Disruptor/ArgumentOutOfRangeException.h>
#include <Disruptor/BasicExecutor.h>
#include <Disruptor/BatchEventProcessor.h>
#include <Disruptor/BlockingQueue.h>
#include <Disruptor/BlockingWaitStrategy.h>
#include <Disruptor/BuildConfig.h>
#include <Disruptor/BusySpinWaitStrategy.h>
#include <Disruptor/ClockConfig.h>
#include <Disruptor/ConsumerRepository.h>
#include <Disruptor/Disruptor.h>
#include <Disruptor/EventHandlerGroup.h>
#include <Disruptor/EventPoller.h>
#include <Disruptor/EventProcessorInfo.h>
#include <Disruptor/ExceptionBase.h>
#include <Disruptor/ExceptionHandlerSetting.h>
#include <Disruptor/ExceptionHandlerWrapper.h>
#include <Disruptor/FatalException.h>
#include <Disruptor/FatalExceptionHandler.h>
#include <Disruptor/FixedSequenceGroup.h>
#include <Disruptor/IConsumerInfo.h>
#include <Disruptor/ICursored.h>
#include <Disruptor/IDataProvider.h>
#include <Disruptor/IEventHandler.h>
#include <Disruptor/IEventProcessor.h>
#include <Disruptor/IEventProcessorFactory.h>
#include <Disruptor/IEventReleaseAware.h>
#include <Disruptor/IEventReleaser.h>
#include <Disruptor/IEventSequencer.h>
#include <Disruptor/IEventTranslator.h>
#include <Disruptor/IEventTranslatorVararg.h>
#include <Disruptor/IExceptionHandler.h>
#include <Disruptor/IExecutor.h>
#include <Disruptor/IgnoreExceptionHandler.h>
#include <Disruptor/IHighestPublishedSequenceProvider.h>
#include <Disruptor/ILifecycleAware.h>
#include <Disruptor/InsufficientCapacityException.h>
#include <Disruptor/InvalidOperationException.h>
#include <Disruptor/ISequence.h>
#include <Disruptor/ISequenceBarrier.h>
#include <Disruptor/ISequenced.h>
#include <Disruptor/ISequencer.h>
#include <Disruptor/ISequenceReportingEventHandler.h>
#include <Disruptor/ITaskScheduler.h>
#include <Disruptor/ITimeoutHandler.h>
#include <Disruptor/IWaitStrategy.h>
#include <Disruptor/IWorkHandler.h>
#include <Disruptor/MultiProducerSequencer.h>
#include <Disruptor/NoOpEventProcessor.h>
#include <Disruptor/NotSupportedException.h>
#include <Disruptor/Pragmas.h>
#include <Disruptor/ProcessingSequenceBarrier.h>
#include <Disruptor/ProducerType.h>
#include <Disruptor/RingBuffer.h>
#include <Disruptor/RoundRobinThreadAffinedTaskScheduler.h>
#include <Disruptor/Sequence.h>
#include <Disruptor/SequenceGroups.h>
#include <Disruptor/Sequencer.h>
#include <Disruptor/SingleProducerSequencer.h>
#include <Disruptor/SleepingWaitStrategy.h>
#include <Disruptor/SpinWait.h>
#include <Disruptor/SpinWaitWaitStrategy.h>
#include <Disruptor/ThreadPerTaskScheduler.h>
#include <Disruptor/ThreadHelper.h>
#include <Disruptor/TimeoutBlockingWaitStrategy.h>
#include <Disruptor/TimeoutException.h>
#include <Disruptor/TypeInfo.h>
#include <Disruptor/Util.h>
#include <Disruptor/WorkerPool.h>
#include <Disruptor/WorkerPoolInfo.h>
#include <Disruptor/WorkProcessor.h>
#include <Disruptor/YieldingWaitStrategy.h>


struct LongEvent {
    long value;
};

class PrintingEventHandler : public Disruptor::IEventHandler< LongEvent > {
public:
    explicit PrintingEventHandler(int toProcess)
            : m_actuallyProcessed(0), m_toProcess(toProcess) {}

    void onEvent(LongEvent& event, int64_t, bool) override
    {
        std::cout << "Event: " << event.value << std::endl;

        if (++m_actuallyProcessed == m_toProcess)
            m_allDone.notify_all();
    }

    void waitEndOfProcessing() // join
    {
        std::unique_lock<decltype(m_mutex)> lk(m_mutex);
        m_allDone.wait(lk);
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_allDone;
    int m_toProcess;
    int m_actuallyProcessed;
};

int main() {
    auto const ExpectedNumberOfEvents = 10000;
    auto const RingBufferSize = 1024;

    // Instantiate and start the disruptor
    auto eventFactory = []() { return LongEvent(); };
    auto taskScheduler = std::make_shared< Disruptor::ThreadPerTaskScheduler >();

    auto disruptor = std::make_shared< Disruptor::disruptor<LongEvent>>(eventFactory, RingBufferSize, taskScheduler);
    std::shared_ptr<Disruptor::IEventHandler<LongEvent>> printingEventHandler = std::make_shared< PrintingEventHandler>(ExpectedNumberOfEvents);

    disruptor->handleEventsWith(printingEventHandler);

    taskScheduler->start();
    disruptor->start();

    // Publish events
    auto ringBuffer = disruptor->ringBuffer();
    for (auto i = 0; i<ExpectedNumberOfEvents; ++i)
    {
        auto nextSequence = ringBuffer->next();
        (*ringBuffer)[nextSequence].value = i;
        ringBuffer->publish(nextSequence);
    }

    // Wait for the end of execution and shutdown
    static_cast<PrintingEventHandler*>(printingEventHandler.get())->waitEndOfProcessing();

    disruptor->shutdown();
    taskScheduler->stop();

    return 0;
}
