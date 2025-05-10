# docker build --network=host -t redstone_base:latest .
FROM ekzm/centos7.2.1511:latest
USER root

ENV pkg_dir=/opt/packages TERM=xterm-256color

COPY deploy_data /deploy_data

RUN rm -rf /etc/yum.repos.d/*.repo \
    && cp /deploy_data/CentOS-Base.repo /etc/yum.repos.d/CentOS-Base.repo \
    && mkdir -p ~/.ssh \
    && rm -f ~/.ssh/* \
    && chmod 700 ~/.ssh \
    && cp /deploy_data/id_rsa* ~/.ssh/ \
    && chmod 600 ~/.ssh/id_rsa \
    && chmod 640 ~/.ssh/id_rsa.pub \
    && cp -r /deploy_data/supervisor /etc/ \
    && chmod 777 /tmp

# exit 0 means ignore error
RUN rpm --rebuilddb                                \
    && yum clean all                               \
    && yum makecache                               \
    && yum update -y; exit 0
RUN yum install -y epel-release python3 wget curl      \
        file rsync net-tools perl-ExtUtils-MakeMaker   \
        bison flex centos-release-scl bzip2 zlib-devel \
        libtool gettext-devel openssl-devel openssl vim

# set gcc7 environment
RUN yum makecache && yum install -y devtoolset-7
# ENV CC="/opt/rh/devtoolset-7/root/usr/bin/gcc"
# ENV CXX="/opt/rh/devtoolset-7/root/usr/bin/g++"
# ENV AR="/opt/rh/devtoolset-7/root/usr/bin/ar"
# ENV NM="/opt/rh/devtoolset-7/root/usr/bin/nm"
# ENV LD="/opt/rh/devtoolset-7/root/usr/bin/ld"
# RUN echo "source /opt/rh/devtoolset-7/enable" > /etc/profile.d/gcc7.sh  \
    # && echo "export CC=${CC}" >> /etc/profile.d/gcc7.sh  \
    # && echo "export CXX=${CXX}" >> /etc/profile.d/gcc7.sh  \
    # && echo "export AR=${AR}" >> /etc/profile.d/gcc7.sh  \
    # && echo "export NM=${NM}" >> /etc/profile.d/gcc7.sh  \
    # && echo "export LD=${LD}" >> /etc/profile.d/gcc7.sh

# install git
RUN yum install -y http://opensource.wandisco.com/centos/7/git/x86_64/wandisco-git-release-7-2.noarch.rpm \
    && yum makecache && yum update -y git
RUN echo "StrictHostKeyChecking=no" > /root/.ssh/config     \
    && echo "UserKnownHostsFile=/dev/null" >> /root/.ssh/config

# install bazelisk
RUN wget "http://storage.jd.local/mirror.bazel.build/bazelisk" -O /usr/bin/bazelisk \
    && chmod a+x /usr/bin/bazelisk

# set locale
RUN localedef -c -i en_US -f UTF-8 en_US.UTF-8
ENV LANG="en_US.UTF-8"
ENV LC_ALL="en_US.UTF-8"
RUN echo "LANG=${LANG}" > /etc/locale.conf  \
    && echo "export LANG=${LANG}" > /etc/profile.d/locale.sh  \
    && echo "export LC_ALL=${LC_ALL}" >> /etc/profile.d/locale.sh

# set timezone
RUN printf "5\n9\n1\n1\n" | tzselect
ENV TZ='Asia/Shanghai'
RUN echo "export TZ=${TZ}" > /etc/profile.d/tz.sh

# prepare python environment
RUN pip3 install /deploy_data/pip/*

# upgrade libstdc++.so
RUN wget "http://mirrors.sr.jd.local/lib/libstdc++.so.6.0.26" -O /lib64/libstdc++.so.6.0.26 \
    && chmod 755 /lib64/libstdc++.so.6.0.26 && ln -sf /lib64/libstdc++.so.6.0.26 /lib64/libstdc++.so.6

ENV REDSTONE_PATH=/App/redstone_server REDSTONE_LOG_PATH=/export/Logs/redstone_server

RUN mkdir -p $REDSTONE_PATH $REDSTONE_LOG_PATH

RUN rm -rf /deploy_data

ENTRYPOINT ["supervisord", "-n"]
