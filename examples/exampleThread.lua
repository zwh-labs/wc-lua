local config = wcConfiguration.new()
config:setDevicePath("/dev/ttyACM0")
config:setWheel( 0, 1024 )
config:setWheel( 1, 1024 )
print( config )

local connection = wcConnection.open( config )
print( connection )
