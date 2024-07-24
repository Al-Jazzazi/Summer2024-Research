# Setup
mkdir exports
cd exports
pip3 install h5py

# Download SIFT (1000000 x 128)
wget ftp://ftp.irisa.fr/local/texmex/corpus/sift.tar.gz
tar -xzvf sift.tar.gz
rm sift.tar.gz

# Download GIST (1000000 x 960)
wget ftp://ftp.irisa.fr/local/texmex/corpus/gist.tar.gz
tar -xzvf gist.tar.gz
rm gist.tar.gz

# Download GloVe (1000000 x 200)
mkdir glove
cd glove 
wget https://nlp.stanford.edu/data/glove.twitter.27B.zip
unzip glove.twitter.27B.zip glove.twitter.27B.200d.txt
python3 ../../utils/txt_to_fvecs.py glove.twitter.27B.200d.txt
python3 ../../utils/split_fvecs.py glove.fvecs glove_base.fvecs temp.fvecs 1000000 110000
python3 ../../utils/split_fvecs.py temp.fvecs glove_learn.fvecs glove_query.fvecs 100000 10000
rm glove.twitter.27B.zip glove.twitter.27B.200d.txt glove.fvecs temp.fvecs
cd ..

# Download Deep (1000000 x 256)
wget --no-check-certificate https://www.cse.cuhk.edu.hk/systems/hash/gqr/dataset/deep1M.tar.gz
tar -xzvf deep1M.tar.gz
rm deep1M.tar.gz deep1M/*.lshbox
mv deep1M deep
cd deep
mv deep1M_base.fvecs deep_base.fvecs
mv deep1M_groundtruth.ivecs deep_groundtruth.ivecs
mv deep1M_learn.fvecs deep_learn.fvecs
mv deep1M_query.fvecs deep_query.fvecs
cd ../..