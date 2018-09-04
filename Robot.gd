extends Skeleton

# class member variables go here, for example:
# var a = 2
# var b = "textvar"
var time = 0

func _ready():
	# Called when the node is added to the scene for the first time.
	# Initialization here
	print("Bone count: {0}".format([get_bone_count()]))
	
	print("Bones:")
	for i in range(get_bone_count()):
		print("{0}\t{1}".format([i, get_bone_name(i)]))

func _process(delta):
	# Called every frame. Delta is time since last frame.
	# Update game logic here.
	time += delta
	
	var transform = Transform()
	transform = transform.translated( Vector3(0, time * 10, 0) )
	
	var bone_id = find_bone("neck01")
	set_bone_pose( bone_id, transform )
