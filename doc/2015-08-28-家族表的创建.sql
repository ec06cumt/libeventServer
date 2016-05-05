USE [uu278]
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
--家族列表
CREATE TABLE [tbluGuild]
(
	[GNum] [int] IDENTITY(1,1) NOT NULL,											--家族编号
	[GName] [varchar](32) COLLATE Chinese_PRC_CI_AS NOT NULL,						--家族名称
	[GMark] [tinyint] NULL,															--家族图标编号（保留）
	[GType] [tinyint] NULL,															--家族类型（保留）
	[GLevel] [tinyint] NOT NULL CONSTRAINT [DF_tbluGuild_GLevel] DEFAULT(1),		--家族等级，默认1级
	[CreateTime] [datetime] NULL CONSTRAINT [DF_tbluGuild_CreateTime] DEFAULT(getdate()),--家族创建时间默认当前时间
	[UpdateTime] [datetime] NULL,													--家族信息更新时间
	[bRefuseJoin] [tinyint] NOT NULL CONSTRAINT [DF_tbluGuild_GRefuse] DEFAULT(0),	--是否拒绝玩家加入家族
	CONSTRAINT [PK_tbluGuild] PRIMARY KEY CLUSTERED									--家族主键
	(
		[GNum]
	)WITH FILLFACTOR = 90 ON [PRIMARY]												--设置填充因子,加快读取速度
) ON [PRIMARY]

GO
--家族成员列表
CREATE TABLE [tbluGuildMember]
(
	[GNum] [int]  NOT NULL,															--家族编号
	[UserID] [int] NOT NULL,														--玩家标识ID
	[Grade] [tinyint] NOT NULL,														--玩家职位 1:家族族长 2：普通成员
	[JoinTime] [datetime] NULL CONSTRAINT [DF_tbluGuildM_JoinTime] DEFAULT(getdate()),--加入家族时间
	[UpdateTime] [datetime] NULL,													--成员信息更新时间
	[GMoney]	[int] NOT NULL CONSTRAINT [DF_tbluGuildM_GMoney] DEFAULT(0),		--成员默认的银两
	CONSTRAINT [PK_tbluGuildMember] PRIMARY KEY CLUSTERED									--家族成员主键
	(
		[UserID]
	)WITH FILLFACTOR = 90 ON [PRIMARY]												--设置填充因子,加快读取速度
) ON [PRIMARY]

GO
--申请加入家族记录
CREATE TABLE [tbluGuildApplication]
(
	[UserID] [int] NOT NULL,														--玩家标识ID
	[GNum] [int]  NOT NULL,															--家族编号
	[Grade] [tinyint] NOT NULL,														--玩家职位 1:家族族长 2：普通成员
	[ApplicationTime] [datetime] NULL CONSTRAINT [DF_tbluGuildA_AppTime] DEFAULT(getdate()),--加入家族时间
	[BApproved] [int] NOT NULL CONSTRAINT [DF_tbluGuildA_BApproved] DEFAULT(0),	    --是否审批通过 0:未审批 1：已经审批												
	[ApprovedTime]	[datetime] NULL,												--审批时间
	CONSTRAINT [PK_tbluGuildApplication] PRIMARY KEY CLUSTERED									--家族申请加入主键
	(
		[UserID],
		[GNum]
	)WITH FILLFACTOR = 90 ON [PRIMARY]												--设置填充因子,加快读取速度
) ON [PRIMARY]

GO

-----------------------------------------------------------

--创建家族信息
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_NULLS ON
GO

CREATE PROCEDURE [GSP_GG_Guild_Create]
	@iUserID int,				--玩家唯一标识
	@iGrade  tinyint,			--玩家职位 ,族长,成员
	@iGNum	 int OUTPUT,        --家族编号
	@szGName  varchar(32),		--家族名称
	@iGMark	 tinyint,			--家族图标编号
	@iGType  tinyint,			--家族类型
	@iGLevel tinyint,			--家族等级
	@bRefuseJoin tinyint		--是否拒绝玩家加入
AS
	SET NOCOUNT ON
	BEGIN TRAN
	--家族名称是否已经被用
	IF EXISTS(SELECT GNum FROM tbluGuild WHERE GName = @szGName)
	BEGIN 
		ROLLBACK TRAN
		RETURN -1
	END
	--已经存在在家族表中,已经有家族,不能创建家族
	IF EXISTS(SELECT UserID FROM tbluGuildMember WHERE UserID = @iUserID)
	BEGIN 
		ROLLBACK TRAN
		RETURN -2
	END
	--用户不存在
	IF EXISTS(SELECT UserID FROM tbluAccount WHERE UserID = @iUserID)
	BEGIN 
		ROLLBACK TRAN
		RETURN -3
	END
	--创建家族
    INSERT tbluGuild(GName,GMark,GType,GLevel,UpdateTime,bRefuseJoin)
	VALUES (@szGName,@iGMark,@iGType,@iGLevel,GETDATE(),@bRefuseJoin)

	IF 0 <> @@ERROR
	BEGIN
		ROLLBACK TRAN
		RETURN @@ERROR
	END

    SET @iGNum = @@IDENTITY
	--把玩家信息加入到家族成员表中
	INSERT tbluGuildMember (GNum,UserID,Grade,UpdateTime)
	VALUES (@iGNum,@iUserID,@iGrade,GETDATE())

	IF 0 <> @@ERROR
	BEGIN
		ROLLBACK TRAN
		RETURN @@ERROR
	END

	COMMIT TRAN
	RETURN 0

SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER ON

GO
SET QUOTED_IDENTIFIER OFF
GO
SET ANSI_NULLS ON
GO
--------------------------------------------------------
--删除家族
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_NULLS ON
GO

CREATE PROCEDURE [GSP_GG_Guild_Delete]
	@iGNum int					--家族标识
AS

	SET NOCOUNT ON
	BEGIN TRAN

	IF NOT EXISTS(SELECT GNum FROM tbluGuild WHERE GNum = @iGNum)
	BEGIN
		ROLLBACK TRAN
		RETURN -1
	END
	--删除家族信息
	DELETE tbluGuild WHERE GNum = @iGNum
	IF 0 <> @@ERROR
	BEGIN
		ROLLBACK TRAN
		RETURN @@ERROR
	END
	--删除家族成员信息
	DELETE tbluGuildMember WHERE GNum = @iGNum
	IF 0 <> @@ERROR
	BEGIN
		ROLLBACK TRAN
		RETURN @@ERROR
	END
	--删除家族申请记录
	DELETE tbluGuildApplication WHERE GNum = @iGNum
	IF 0 <> @@ERROR
	BEGIN
		ROLLBACK TRAN
		RETURN @@ERROR
	END

	COMMIT TRAN
	RETURN 0
	
GO
SET QUOTED_IDENTIFIER OFF
GO
SET ANSI_NULLS ON
GO

-----------------------------------------------------------

--修改家族名称信息
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_NULLS ON
GO

CREATE PROCEDURE [GSP_GG_Guild_UpdateName]
	@iGNum int,					--家族标识
	@szGName varchar(32)        --家族名称
AS

	SET NOCOUNT ON
	IF NOT EXISTS(SELECT GNum FROM tbluGuild WHERE GNum = @iGNum)
	BEGIN
		RETURN -1
	END
	
	--修改家族名称
	UPDATE tbluGuild
	SET GName = @szGName
	WHERE GNum = @iGNum
	IF 0 <> @@ERROR
	BEGIN
		RETURN @@ERROR
	END

	RETURN 0
	
GO
SET QUOTED_IDENTIFIER OFF
GO
SET ANSI_NULLS ON
GO

-----------------------------------------------------------
--修改家族是否允许玩家加入
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_NULLS ON
GO

CREATE PROCEDURE [GSP_GG_Guild_UpdateRefuseJoin]
	@iGNum int,					--家族标识
	@bRefuseJoin tinyint
AS

	SET NOCOUNT ON
	IF NOT EXISTS(SELECT GNum FROM tbluGuild WHERE GNum = @iGNum)
	BEGIN
		RETURN -1
	END
	
	--修改家族名称
	UPDATE tbluGuild
	SET bRefuseJoin = @bRefuseJoin
	WHERE GNum = @iGNum
	IF 0 <> @@ERROR
	BEGIN
		RETURN @@ERROR
	END

	RETURN 0
	
GO
SET QUOTED_IDENTIFIER OFF
GO
SET ANSI_NULLS ON
GO




