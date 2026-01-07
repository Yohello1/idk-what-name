Hello guys this is my plan to build a ray tracer + sph fluid sim. Where the fluid simulation & geometry construction is done on the server, rendering done on client.

# Why not on same computer?
Fluid simulation expensive, doing that + ray tracing is going to be sadness.

So offload it to remote server, also has the benifit of allowing multiple people being able to view the fluid simulation with custom fiedelity settings.

# How

