mm2capture uses SQLite database to store captured sessions. Every session contains an airtraffic profile captured in the time interval. Replaying is just a reading message by message from the store and sending them to the output (see supported i/o types below), preserving intervals between them in with nanoseconds resolution.

Supported I/O types:
Now only tcp client output supported.

TODO
- add Win platform-specific high-resolution version of sleep() (nanosleep() used in Linux version)
- add logging with selectable level of verbosity
