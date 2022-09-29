Take a test

# This is test code for updaing PVD data to gwangju control center
## copy to the catkin_ws/src to compile 
## See what happens
```bash
cd catkin_ws/src
git clone https://github.com/kim3957/ldm_test_gwangju.git
../
catkin_make
rosrun test pub
rosrun test recv
```
## For recving intersection informations
```
rostopic echo LDM_data
```
