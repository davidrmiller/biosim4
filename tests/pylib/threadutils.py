#!/usr/bin/env python3
# encoding: utf-8

""" Thread utility functions
    """

import threading
import weakref
import inspect
import traceback

# threading functions
def start_thread(thread_func,tname='',targs=()):
    """ start a new thread to execute the supplied function
        """
    thread = threading.Thread(None, thread_func,name=tname,args=(targs,))
    thread.daemon = True
    thread.start()
    return thread

def stop_thread(thread_obj):
    """ stop thread execution
        """
    result = thread_obj.join(3.0)
    alive = thread_obj.isAlive()
    return (result,alive)


class Signal():
    """ Callback functions (so called slots) can be connected
        to a signal and will be called when the signal is called
        (Signal implements __call__).
        The slots receive two arguments: the sender of the signal and a custom data object. Two different threads won't be allowed to send signals at the same time application-wide,
        concurrent threads will have to wait until the lock is
        releaesed again.
        The lock allows recursive reentry of the same thread to
        avoid deadlocks when a slot wants to send a signal itself.
        """

    _lock = threading.RLock()
    signal_error = None

    def __init__(self):
        self._functions = weakref.WeakSet()
        self._methods = weakref.WeakKeyDictionary()

        # the Signal class itself has a static member signal_error
        # where it will send tracebacks of exceptions that might
        # happen.
        # Here we initialize it if it does not exist already
        if not Signal.signal_error:
            Signal.signal_error = 1
            Signal.signal_error = Signal()

    def connect(self, slot):
        """ connect a slot to this signal.
        
        The parameter slot can be a funtion that takes exactly 2 arguments or a method that takes self plus 2 more
        arguments, or it can even be even another signal. the first argument is a reference to the sender of the signal and the second argument is the payload. The payload can be anything, it totally depends on the sender and type of the signal.
        """
        if inspect.ismethod(slot):
            if slot.__self__ not in self._methods:
                self._methods[slot.__self__] = set()
            self._methods[slot.__self__].add(slot.__func__)
        else:
            self._functions.add(slot)

    def __call__(self, sender, data, error_signal_on_error=True):
        """ dispatch signal to all connected slots.
        
        This is a synchronous operation, It will not return before all slots have been called.
        Also only exactly one thread is allowed to emit signals at any time, all other threads that try to emit *any* signal anywhere in the application at the same time will be blocked until the lock is released again. The lock will allow recursive re-entry of the same thread, this means a slot can itself emit other signals before it returns (or
        signals can be directly connected to other signals) without problems.
        If a slot raises an exception a traceback will be sent to the static Signal.signal_error() or to logging.critical()"""
        with self._lock:
            sent = False
            errors = []
            #print "signal: ", sender
            for func in self._functions:
                try:
                    func(sender, data)
                    #print "signal sent 1"
                    sent = True
                except:
                    errors.append(traceback.format_exc())

            for obj, funcs in self._methods.items():
                for func in funcs:
                    try:
                        func(obj, sender, data)
                        #print "signal sent 2"
                        sent = True
                    except:
                        errors.append(traceback.format_exc())

            for error in errors:
                if error_signal_on_error:
                    Signal.signal_error(self, (error), False)
                    #print "error: ", error
                else:
                    logging.critical(error)

            return sent

class Timer(Signal):
    """ a simple timer (used for stuff like keepalive) """

    def __init__(self, interval):
        """ create a new timer, interval is in seconds"""
        Signal.__init__(self)
        self._interval = interval
        self._timer = None
        self._start()

    def _fire(self):
        """ fire the signal and restart it"""
        self.__call__(self, None)
        self._start()

    def _start(self):
        """ start the timer"""
        self._timer = threading.Timer(self._interval, self._fire)
        self._timer.daemon = True
        self._timer.start()

    def cancel(self):
        """ cancel the timer"""
        self._timer.cancel()