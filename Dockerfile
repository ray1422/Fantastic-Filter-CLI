FROM nvidia/cuda:10.1-runtime-ubuntu16.04
WORKDIR /fantastic-filter-cli
ENV TF_CPP_MIN_LOG_LEVEL 3
ENV FF_MODEL_PATH /fantastic-filter-cli/FF_Model
RUN apt update
RUN apt install -y gcc wget git make
RUN rm ./* -rf
RUN git clone https://github.com/ray1422/Fantastic-Filter-CLI.git ./
RUN git checkout master
RUN wget https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-gpu-linux-x86_64-2.3.0.tar.gz -O tf.tar.gz -q
RUN tar -C /usr/local -xzf tf.tar.gz
RUN make main
RUN ldconfig
RUN ln -s ./bin/fantastic-filter-cli /bin/fantastic-filter-cli
RUN ln -s /bin/fantastic-filter-cli /bin/ff-cli
RUN wget -O FF_Model.tar.gz https://github.com/ray1422/Fantastic-Filter-Professional-Plus/releases/download/v0.0.1/FF_Model.tar.gz
RUN tar -xvf FF_Model.tar.gz

