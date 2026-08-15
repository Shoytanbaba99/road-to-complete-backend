## 💡 Key Takeaways

There are two types of Time, Wall Clock Time and CPU monotonic time.

Wall clock is prone to changes in the system time, such as daylight saving time adjustments or manual changes to the system clock or even ntp adjusting it over network automatically, while monotonic time is not affected by these changes and always moves forward at a constant rate.
Cpu monotonic time is a measure of the time elapsed since an arbitrary point in the past, system boot time mainly, and is not affected by changes in the system clock. It is typically used for measuring time intervals and for scheduling tasks that need to be executed at specific intervals.
