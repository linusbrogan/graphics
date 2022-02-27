

Functionality has been added to create new xwdmaps and
plot pixels directly into them (and save them as files) without
going through FPT and the windowing system.  There is really
only the ability to plot and sense points (no line drawing,
polygon filling, etc) but this could be useful for people
trying to raytrace gigantic files or doing things in parallel
where the output should be to files and the windowing system
is just in the way.

Latest version changes names of fgetint and fputint
to avoid naming conflicts with FPToolkit.c


