include $(TOPDIR)/rules.mk

PKG_NAME:=qic1832_sdk
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

PKG_BUILD_PARALLEL:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/OpenQCam/qic1832_sdk.git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=1bbc386c4ca8a95285375af8b2f5eea5bdb02454
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  CATEGORY:=QIC1832 SDK
  TITLE:=SDK for QIC1832 encoding camera.
  DEFAULT:=y
  URL:=
  DEPENDS:=\
	+kmod-video-core \
	+kmod-video-uvc \
	+librt
endef

define Package/$(PKG_NAME)_util
  CATEGORY:=QIC1832 SDK
  TITLE:=Utility of SDK for QIC1832 encoding camera.
  DEFAULT:=y
  URL:=
  DEPENDS:=+$(PKG_NAME) +librt
endef

define Build/Configure
	$(call Build/Configure/Default,--with-linux-headers=$(LINUX_DIR))
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/qic_api/qic_include
	$(CP) $(PKG_BUILD_DIR)/qic_api/qic_include $(1)/usr/include/qic_api
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/*.a $(1)/usr/lib
endef

define Package/$(PKG_NAME)/install
endef

define Package/$(PKG_NAME)_util/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) -D $(PKG_BUILD_DIR)/bin/* $(1)/usr/sbin
endef


$(eval $(call BuildPackage,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)_util))
