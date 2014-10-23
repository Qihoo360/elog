## Overview

A erlang log library nif

simple and fast

## Feature
* Six grained log levels(trace, debug, info, warn, error, fatal)
* Support configred log level

## Config

```
 {elog, [
    {logname, "bada"},
    {log_level, 4} 
    ]},
```

## Usage

    elog_backend:warn("This is a log"),
    elog_backend:trace("This is a trace log"),
