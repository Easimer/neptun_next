## neptun\_next

Prints your current or next class like this:
```
⏩09:30AM Analízis I. (ABCDEF...) @ A.105
```

Example configuration ($HOME/.config/neptun_next.cfg)
```
ics_url = https://neptun.uni-versity.hu:443/hallgato/cal/cal.ashx?id=XXXXXXXXXXXXXXXXXXXXX.ics
```

If you don't want neptun_next to do a HTTP request every second then you can use a local file
```
ics_url = file:///home/user/.cache/neptun_next.ics
```

### Building
The only requirements to build are make and the development libraries for cURL.
However if you want to build and run the tests you will need to install check/libcheck.

To build the main executable, simply run
```
make
```

To build the tests and run them:
```
make BUILD_TESTS=1
```

To run the tests:
```
make test
OR
tests/run_tests
```
