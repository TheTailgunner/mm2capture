mm2capture uses SQLite database to store captured frames. Session contains an airtraffic profile captured in the time interval. Replaying is just reading message by message from the database and sending them to the output (see supported I/O types below), preserving time intervals between them with nanoseconds resolution.

Supported I/O types:
- TCP client

TODO
- implement high-resolution version of sleep() on Windows (nanosleep() was used in Linux version)
- add logging with adjustable level of verbosity
