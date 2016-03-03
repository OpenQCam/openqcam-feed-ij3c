include $(TOPDIR)/rules.mk

PKG_NAME:=ij3c
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  CATEGORY:=QIC1832 SDK
  TITLE:=IJ3C platform 
  DEFAULT:=y
  HIDDEN:=1
  DEPENDS:= \
	+@CONFIG_LIBCURL_COOKIES \
	+@CONFIG_LIBCURL_FILE \
	+@CONFIG_LIBCURL_FTP \
	+@CONFIG_LIBCURL_HTTP \
	+@CONFIG_LIBCURL_NO_SMB \
	+@CONFIG_LIBCURL_POLARSSL \
	+@CONFIG_LIBCURL_PROXY \
	+@CONFIG_OPENSSL_WITH_EC \
	+alsa-lib \
	+iperf \
	+kmod-input-core \
	+kmod-sound-core \
	+kmod-usb-audio \
	+kmod-usb-uhci \
	+kmod-video-core \
	+kmod-video-uvc \
	+kmod-video-videobuf2 \
	+libcurl \
	+libiwinfo-lua \
	+liblua \
	+libopenssl \
	+libpolarssl \
	+libpthread \
	+libreadline \
	+librt \
	+libubus-lua \
	+libuci-lua \
	+libustream-polarssl \
	+lua \
	+luci \
	+luci-app-firewall \
	+luci-base \
	+luci-lib-ip \
	+luci-lib-nixio \
	+luci-mod-admin-full \
	+luci-proto-ipv6 \
	+luci-proto-ppp \
	+luci-ssl \
	+luci-theme-bootstrap \
	+px5g \
	+rpcd \
	+uclibcxx \
	+uhttpd \
	+uhttpd-mod-ubus \
	+zlib
  	
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(PKG_BUILD_DIR)/prodtest.cgi prodtest.c
endef

define Package/$(PKG_NAME)/install
	if [ -d "./files" ]; then \
		$(CP) ./files/* $(1); \
	fi
	$(INSTALL_DIR) $(1)/www/cgi-bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/prodtest.cgi $(1)/www/cgi-bin/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))

