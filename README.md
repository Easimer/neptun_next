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

