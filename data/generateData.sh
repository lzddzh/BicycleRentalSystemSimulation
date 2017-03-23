# playtime at a, b, c, d
num=100000
python normal.py 29.90766 5.11292 $num a 0
python normal.py 29.982 5.2889 $num b 0
python normal.py 30.03 4.96338 $num c 0 
python normal.py 30.03283 5.0116170 $num d 0

# create interval at a, b, c, d
num=100000
python exponential.py 8.6037 $num a 0
python exponential.py 8.4168 $num b 0
python exponential.py 8.9514 $num c 0
python exponential.py 7.9376 $num d 0

num=100000
python beta_with_trans.py 3.68 3.78 $num a 0
python beta_with_trans.py 3.68 3.78 $num b 0
python beta_with_trans.py 3.68 3.78 $num c 0
python beta_with_trans.py 3.68 3.78 $num d 0
