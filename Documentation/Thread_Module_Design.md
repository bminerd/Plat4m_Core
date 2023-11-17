# Plat4m Thread Module

The Plat4m::Thread module handles abstraction of an OS thread or task. Unlike most of the modules in Plat4m, Threads aren't created by instantiating the implementation class' constructor (ex. ThreadLinux()). In order to make Threads as generic and easy to use as possible, Threads are designed to be created by calling System::createThread() which returns a reference to a Thread object that has been created on the heap (more about Plat4m's safe dynamic memory allocation [here](Fix_Me)). This means that a Thread can be created at any layer in a Plat4m application, not just at the Application Layer.

For example, if you've chosen and instantiated a SystemFreeRtos object as your System driver, when you call:

``` C++
Thread& thread = System::createThread(&threadCallbackFunction);
```

This method ultimately gets called:

``` C++
Thread& SystemFreeRtos::driverCreateThread(Thread::RunCallback& callback,
                                       	   const TimeMs periodMs,
                                       	   const uint32_t nStackBytes)
{
    return *(new ThreadFreeRtos(callback, periodMs, nStackBytes));
}
```

In this case, the ThreadFreeRtos() constructor calls the necessary FreeRTOS functions to create a task and point it to the callback function you specified in the System::createThread(). If you had instantiated a SystemLinux object as your System Driver, a Linux thread would be created on the back end.

Obviously there are some underlying system dependencies, as you can't run FreeRTOS or Linux on every processor. However, abstracting the underlying OS is a very powerful tool that enables:

- Changing the OS based on product needs (ex. safety-certified OS for one family of products, non-safety-certified OS for another family)
- Running a single code base against multiple operating systems (ex. test and develop on a Linux computer, but deploy to an ARM microcontroller for the final product)

One of the benefits of using the Thread module is that it allows you to write a single function or method that can be called from a simple cooperative run-to-completion scheduler, a preemptive run-to-completion scheduler, or a fully-preemptive scheduler (like a traditional RTOS or higher-level OS like Linux or Windows). For this to work, a Thread's callback function or method must be written in a run-to-completion fashion. For example, a function written for a traditional preemptive OS might be written like:

``` C++
void threadCallbackRtos()
{
    while (true)
    {
        waitForEvent();

        doSomething();
    }
}
```

To write this in a Plat4m::Thread compatible way, this function would now become:

``` C++
void threadCallbackPlat4m()
{
    if (waitForEvent())
    {
    	doSomething();
    }
}
```

If the selected Plat4m::System driver is for a preemptive OS (ex. FreeRTOS), the function or method that calls threadCallback() looks like this:

``` C++
void ThreadFreeRtos::taskCallback(void* parameter)
{
    ThreadFreeRtos* thread = static_cast<ThreadFreeRtos*>(parameter);
    TickType_t lastWakeTime = xTaskGetTickCount();

    while (true) // Loop forever
    {
        if (thread->getPeriodMs() != 0)
        {
		    vTaskDelayUntil(&lastWakeTime, thread->getPeriodMs());
        }

        thread->run();
    }
}
```

The `while (true)` infinite loop in threadCallbackRtos() has essentially been moved up one level to within the Plat4m::ThreadFreeRtos driver. While this may seem like a trivial change, this enables you to use almost any kind of OS under the hood--cooperative, preemptive, run-to-completion, etc.--and still call threadCallbackPlat4m() in exactly the same way.

If you're wondering why you'd want to use a preemptive run-to-completion RTOS, check out the LazerOS project [here](github.com/bminerd/LazerOS).



## API Link

Insert here.

![""](Images/System_Module.svg)
