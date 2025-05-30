#!/bin/bash

set -e

CONFIG_DIR="$HOME/.config/nvim"
BACKUP_DIR="$HOME/.config/nvim_backup_$(date +%s)"

echo "🔧 正在安装 Neovim 配置..."

# 如果已有配置，备份
if [ -d "$CONFIG_DIR" ]; then
  echo "📦 检测到已有配置，备份到 $BACKUP_DIR"
  mv "$CONFIG_DIR" "$BACKUP_DIR"
fi

# 解压配置文件
echo "📁 解压配置到 ~/.config/nvim"
mkdir -p "$HOME/.config"
unzip neovim-config.zip -d "$HOME/.config/"
mv "$HOME/.config/nvim-config" "$CONFIG_DIR"

echo "✅ 安装完成！"
echo "👉 请打开 Neovim 并运行 :Lazy sync 安装插件。"
