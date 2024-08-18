#include "input.hpp"

void input::initialize(lua_State* L)
{
	script_scheduler->add_to_queue(R"(
		local virtual_input_manager = game:GetService("VirtualInputManager")
		local user_input_service = game:GetService("UserInputService")

		getgenv().mouse1click = function()
			virtual_input_manager:SendMouseButtonEvent(0, 0, 0, true, game, 1)
			virtual_input_manager:SendMouseButtonEvent(0, 0, 0, false, game, 1)
		end

		getgenv().mouse1press = function()
			virtual_input_manager:SendMouseButtonEvent(0, 0, 0, true, game, 1)
		end

		getgenv().mouse1release = function()
			virtual_input_manager:SendMouseButtonEvent(0, 0, 0, false, game, 1)
		end

		getgenv().mouse2click = function()
			virtual_input_manager:SendMouseButtonEvent(0, 0, 1, true, game, 1)
			virtual_input_manager:SendMouseButtonEvent(0, 0, 1, false, game, 1)
		end

		getgenv().mouse1press = function()
			virtual_input_manager:SendMouseButtonEvent(0, 0, 1, true, game, 1)
		end

		getgenv().mouse1release = function()
			virtual_input_manager:SendMouseButtonEvent(0, 0, 1, false, game, 1)
		end

		getgenv().mousemoveabs = function(x, y)
			virtual_input_manager:SendMouseMoveEvent(x, y, game)
		end

		getgenv().mousemoverel = function(x, y)
			local current_pos = user_input_service:GetMouseLocation()
			virtual_input_manager:SendMouseMoveEvent(current_pos.X + x, current_pos.Y + y, game)
		end

		getgenv().mousescroll = function(pixels)
			virtual_input_manager:SendMouseWheelEvent(0, 0, pixels > 0, game)
		end

		getgenv().firetouchinterest = function(target, touch_with, on)
			if on == 0 then return end
	
			if target.ClassName == "TouchTransmitter" then
				local function get()
					local class_names = { "BasePart", "Part", "MeshPart" }
					for _, v in pairs(class_names) do
						if target:FindFirstAncestorOfClass(v) then
							return target:FindFirstAncestorOfClass(v)
						end
					end
				end
				target = get()
			end
	
			local old_target_pos = target.CFrame
			local old_target_cc = target.CanCollide
	
			target.CanCollide = false
			target.CFrame = touch_with.CFrame
			task.wait()
			target.CFrame = old_target_pos
			target.CanCollide = old_target_cc
		end

		getgenv().fireclickdetector = function(object, distance, event)
			local click_detector = object:FindFirstChild("ClickDetector") or object
			local old_cd_parent = click_detector.Parent
	
			local stub_part = Instance.new("Part")
			stub_part.Transparency = 1
			stub_part.Size = Vector3.new(30, 30, 30)
			stub_part.Anchored = true
			stub_part.CanCollide = false
			stub_part.Parent = workspace
	
			click_detector.Parent = stub_part
			click_detector.MaxActivationDistance = math.huge
	
			local connection = nil
			connection = game:GetService("RunService").Heartbeat:Connect(function()
				stub_part.CFrame = workspace.Camera.CFrame * CFrame.new(0, 0, -20) * CFrame.new(workspace.Camera.CFrame.LookVector.X, workspace.Camera.CFrame.LookVector.Y, workspace.Camera.CFrame.LookVector.Z)
				game:GetService("VirtualUser"):ClickButton1(Vector2.new(20, 20), workspace:FindFirstChildOfClass("Camera").CFrame)
			end)
	
			click_detector.MouseClick:Once(function()
				connection:Disconnect()
				click_detector.Parent = old_cd_parent
				stub_part:Destroy()
			end)
		end

		getgenv().fireproximityprompt = function(target)
			local function get()
				local class_names = { "BasePart", "Part", "MeshPart" }
				for _, v in pairs(class_names) do
					if target:FindFirstAncestorOfClass(v) then
						return target:FindFirstAncestorOfClass(v)
					end
				end
			end
	
			local character = game:GetService("Players").LocalPlayer.Character
			local hold_duration, max_activation_distance, enabled, thing = target.HoldDuration, target.MaxActivationDistance, target.Enabled, target.RequiresLineOfSight
	
			local parent = nil
	
			target.MaxActivationDistance = math.huge
			target.HoldDuration = 0
			target.Enabled = true
			target.RequiresLineOfSight = false
	
			local stub_part = get()
			if not stub_part then
				parent = target.Parent
				target.Parent = Instance.new("Part", workspace)
				stub_part = target.Parent
			end
	
			local stub_part_pos = stub_part.CFrame
			stub_part.CFrame = character.Head.CFrame + character.Head.CFrame.LookVector * 2
			task.wait()
			target:InputHoldBegin()
			task.wait()
			target:InputHoldEnd()
	
			target.HoldDuration = hold_duration
			target.MaxActivationDistance = max_activation_distance
			target.Enabled = enabled
			target.RequiresLineOfSight = thing
			stub_part.CFrame = stub_part_pos
			target.Parent = parent or target.Parent
		end

		local is_window_focused = false
		getgenv().isrbxactive = function()
			return is_window_focused
		end

		user_input_service.WindowFocused:Connect(function()
			is_window_focused = true
		end)

		UserInputService.WindowFocusReleased:Connect(function()
			is_window_focused = false
		end)
	)");
}